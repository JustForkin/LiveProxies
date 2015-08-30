#define _BSD_SOURCE

#include "Harvester.h"
#include "Logger.h"
#include "ProxyLists.h"
#include "IPv6Map.h"
#include "Global.h"
#include "Config.h"
#include <python2.7/Python.h>
#include <dirent.h>
#include <openssl/sha.h>

static char *last_strstr(const char *haystack, const char *needle)
{
	if (*needle == '\0')
		return (char *)haystack;

	char *result = NULL;
	for (;;) {
		char *p = strstr(haystack, needle);
		if (p == NULL)
			break;
		result = p;
		haystack = p + 1;
	}

	return result;
}

void HarvestLoop()
{
	// holy memory

	Py_Initialize();
	HarvesterPrxsrcStats = NULL;
	SizeHarvesterPrxsrcStats = 0;

	for (;;) {
		PyObject *pName, *pModule, *pFunc = NULL, *pResult;
		bool statsEntryAdded = false;

		PyRun_SimpleString("import sys");

		char *sysPath = malloc(19 + strlen(HarvestersPath) + 1); {
			sprintf(sysPath, "sys.path.append(\"%s\")", HarvestersPath);
			PyRun_SimpleString(sysPath);
		} free(sysPath);

		DIR *d;
		struct dirent *ent;
		d = opendir(HarvestersPath);
		if (!d) {
			Log(LOG_LEVEL_ERROR, "Failed to open \"%s\", no proxies added", HarvestersPath);
			goto end;
		}
		while ((ent = readdir(d)) != NULL) {
			if (ent->d_type != DT_REG /* normal file */ || strlen(ent->d_name) < 4)
				continue;

			HARVESTER_PROXY_SOURCE_TYPE sourceType = NONE;
			size_t fileNameLen = strlen(ent->d_name);
			if (strcmp(ent->d_name + fileNameLen - 3, ".py") == 0)
				sourceType = SCRIPT;
			if (strcmp(ent->d_name + fileNameLen - 4, ".txt") == 0 || strcmp(ent->d_name + fileNameLen - 4, ".prx") == 0)
				sourceType = STATIC;
			if (sourceType == NONE)
				continue;

			char *path = (char*)malloc(10 + strlen(ent->d_name) + 1 /* NULL */);
			sprintf(path, "%s", ent->d_name);
			path[strlen(path) - 3] = '\0';

			Log(LOG_LEVEL_SUCCESS, "Executing %s...", path);

			char *result;
			uint32_t added = 0, addedPrev = 0, total = 0;

			if (sourceType == SCRIPT) {
				pName = PyString_FromString(path);
				pModule = PyImport_Import(pName);
				Py_DECREF(pName);

				if (pModule == NULL) {
					PyErr_Print();
					goto freepath;
				}
				pFunc = PyObject_GetAttrString(pModule, "run");
				if (!pFunc) {
					PyErr_Print();
					goto freemodule;
				}
				pResult = PyObject_CallObject(pFunc, NULL);
				if (!pResult) {
					PyErr_Print();
					goto freefunc;
				}

				result = PyString_AsString(pResult);
			}
			if (sourceType == STATIC) {
				char pathFull[(strlen(HarvestersPath) + 1 + fileNameLen) * sizeof(char)];
				strcpy(pathFull, HarvestersPath);
				strcat(pathFull, "/");
				strcat(pathFull, ent->d_name);

				FILE *hFile = fopen(pathFull, "r"); {
					if (hFile == NULL)
						goto freepath;
					fseek(hFile, 0, SEEK_END);
					size_t size = ftell(hFile);
					fseek(hFile, 0, SEEK_SET);

					result = malloc(size + 1);
					fread(result, size, 1, hFile);
					result[size] = 0x00;
				} fclose(hFile);
			}

			char *tokSave = NULL;
			char *pch = strtok_r(result, "\n", &tokSave);
			PROXY_TYPE curType = PROXY_TYPE_HTTP;
			uint16_t curPort;
			while (pch != NULL) {
				if (pch[0] == '\0') {
					pch = strtok_r(NULL, "\n", &tokSave);
					continue;
				}
				if (strncmp(pch, "setType", 7) == 0)
					curType = atoll(pch + 8);

				if (ProxyIsSSL(curType) && !SSLEnabled) {
					Log(LOG_LEVEL_WARNING, "Got SSL proxy, but SSL is disabled");
					goto next;
				}

				char *delimiterOffset = last_strstr(pch, ":");

				if (delimiterOffset == NULL)
					goto next;

				curPort = atoi(delimiterOffset + 1);
				if (curPort == 0)
					goto next;

				pch[delimiterOffset - pch] = '\0';

				IPv6Map *map;
				if (pch[0] == '[' && pch[strlen(pch) - 1] == ']') {
					pch[strlen(pch) - 1] = '\0';
					map = StringToIPv6Map(pch + 1);
				} else
					map = StringToIPv6Map(pch);

				if (map == NULL)
					goto next;

				IP_TYPE type = GetIPType(map);

				if (GlobalIp4 == NULL && type == IPV4) {
					Log(LOG_LEVEL_WARNING, "Got IPv4 address from harvester %s, but no IPv4 is provided (GlobalIp4)", path);
					free(map);
					goto next;
				}
				if (GlobalIp6 == NULL && type == IPV6) {
					Log(LOG_LEVEL_WARNING, "Got IPv6 address from harvester %s, but no IPv6 is provided (GlobalIp6)", path);
					free(map);
					goto next;
				}

				UNCHECKED_PROXY *up = AllocUProxy(map, curPort, curType, NULL, NULL);

				addedPrev = added;
				added += UProxyAdd(up);

				if (addedPrev == added)
					UProxyFree(up);
				total++;
next:
				pch = strtok_r(NULL, "\n", &tokSave);
			}

			HARVESTER_PRXSRC_STATS_ENTRY entry;
			entry.name = path;
			entry.added = total;
			entry.addedNew = added;
			entry.type = sourceType;

			pthread_mutex_lock(&LockHarvesterPrxsrcStats); {
				if (SizeHarvesterPrxsrcStats < ProxySourcesBacklog) {
					HarvesterPrxsrcStats = SizeHarvesterPrxsrcStats == 0 ? malloc(++SizeHarvesterPrxsrcStats * sizeof(HARVESTER_PRXSRC_STATS_ENTRY)) :
						realloc(HarvesterPrxsrcStats, ++SizeHarvesterPrxsrcStats * sizeof(HARVESTER_PRXSRC_STATS_ENTRY));
					memcpy(&(HarvesterPrxsrcStats[SizeHarvesterPrxsrcStats - 1]), &entry, sizeof(HARVESTER_PRXSRC_STATS_ENTRY));
				} else {
					free(HarvesterPrxsrcStats[SizeHarvesterPrxsrcStats].name);
					for (size_t x = SizeHarvesterPrxsrcStats - 1;x < SizeHarvesterPrxsrcStats;x--) {
						memcpy(&(HarvesterPrxsrcStats[x - 1]), &(HarvesterPrxsrcStats[x]), sizeof(HARVESTER_PRXSRC_STATS_ENTRY));
					}
					memcpy(&(HarvesterPrxsrcStats[0]), &entry, sizeof(HARVESTER_PRXSRC_STATS_ENTRY));
				}
			} pthread_mutex_unlock(&LockHarvesterPrxsrcStats);

			statsEntryAdded = true;

			printf("Added %d (%d new) proxies from %s\n", total, added, path);
			if (sourceType == STATIC) {
				free(result);
			} else {
				Py_DECREF(pResult);
freefunc:
				Py_XDECREF(pFunc);
freemodule:
				Py_DECREF(pModule);
			}
freepath:
			if (!statsEntryAdded)
				free(path);
		}
		closedir(d);
		if (SizeUncheckedProxies == 0)
			printf("Warning: no proxies to check, all threads will be inactive\n");
end:
		msleep(HARVEST_TIMEOUT);
	}
}