#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "Interface.h"
#include <libconfig.h>

bool HtmlTemplateUseStock;

typedef struct _HTML_TEMPLATE_MIME_TYPE {
	char *extension;
	char *type;
} HTML_TEMPLATE_MIME_TYPE;

HTML_TEMPLATE_MIME_TYPE *HtmlTemplateMimeTypes;
size_t HtmlTemplateMimeTypesSize;

typedef enum _HTML_TEMPLATE_COMPONENT_IDENTIFIER {
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_VERSION = 0,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CURRENT_PAGE = 1,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_HOME_ACTIVE = 2,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_UPROXIES_ACTIVE = 3,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_PROXIES_ACTIVE = 4,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_SOURCES_ACTIVE = 5,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_STATS_ACTIVE = 6,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_USER = 7,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_COUNT_UPROXIES = 8,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_COUNT_PROXIES = 9,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_UPROXIES_HEAD = 10,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_UPROXIES_TABLE_ITEMS_START = 11,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_UPROXIES_TABLE_ITEMS_END = 12,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_TABLE_ODD = 13,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_TABLE_EVEN = 14,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_TABLE_OK = 15,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_TABLE_WARN = 16,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CFG_TABLE_ERR = 17,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_UPROXIES_ITEM = 18,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PROXIES_HEAD = 19,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PROXIES_TABLE_ITEMS_START = 20,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PROXIES_TABLE_ITEMS_END = 21,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PROXIES_ITEM = 22,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PRXSRC_HEAD = 23,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PRXSRC_TABLE_ITEMS_START = 24,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PRXSRC_TABLE_ITEMS_END = 25,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_PRXSRC_ITEM = 26,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_STATIC = 27,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_TABLE_BREAK = 28,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_STATS_GEO_HEAD = 29,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_STATS_GEO_TABLE_ITEMS_START = 30,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_STATS_GEO_TABLE_ITEMS_END = 31,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_STATS_GEO_ITEM = 32,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_IP = 33,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_PORT = 34,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_TYPE = 35,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_COUNTRY_LOWER = 36,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_COUNTRY_UPPER = 37,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_LIVE_SINCE = 38,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_LAST_CHECKED = 39,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_CONNECT_TIMEOUT = 40,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_HTTP_S_TIMEOUT = 41,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_SUCCESSFUL_CHECKS = 42,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_FAILED_CHECKS = 43,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_RETRIES = 44,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_CHECK_UID = 45,
	HTML_TEMPLATE_COMPONENT_IDENTIFIER_INVALID = -1
} HTML_TEMPLATE_COMPONENT_IDENTIFIER;

typedef struct _HTML_TEMPLATE_COMPONENT {
	HTML_TEMPLATE_COMPONENT_IDENTIFIER identifier;
	void *content;
} HTML_TEMPLATE_COMPONENT;

typedef struct _HTML_TEMPALTE_TABLE_INFO {
	bool inTable;
	size_t currentComponentIteration;
	size_t tableObjectIteration;
	ssize_t tableHeadOrItemIteration;
	void *tableObject;
} HTML_TEMPALTE_TABLE_INFO;

typedef struct _HTML_TEMPLATE_TABLE_STATS_GEO {
	const char *countryCode;
	uint64_t count;
} HTML_TEMPLATE_TABLE_STATS_GEO;

/*HTML_TEMPLATE_COMPONENT *HtmlTemplateDynamic;
size_t HtmlTemplateDynamicSize;*/

// T_VERSION, T_CURRENT_PAGE, T_CFG_HOME_ACTIVE, T_CFG_UPROXIES_ACTIVE, T_CFG_PROXIES_ACTIVE, T_CFG_SOURCES_ACTIVE, T_CFG_STATS_ACTIVE
HTML_TEMPLATE_COMPONENT *HtmlTemplateHead;
size_t HtmlTemplateHeadSize;
// [none]
HTML_TEMPLATE_COMPONENT *HtmlTemplateFoot;
size_t HtmlTemplateFootSize;

// T_USER, T_COUNT_UPROXIES, T_COUNT_PROXIES
HTML_TEMPLATE_COMPONENT *HtmlTemplateHome;
size_t HtmlTemplateHomeSize;

// T_UPROXIES_HEAD[1-5], T_UPROXIES_TABLE_ITEMS_[START/END], T_CFG_TABLE_ODD, T_CFG_TABLE_EVEN, T_CFG_TABLE_OK, T_CFG_TABLE_WARN, T_CFG_TABLE_ERR, T_UPROXIES_ITEM[1-5]
HTML_TEMPLATE_COMPONENT *HtmlTemplateUProxies;
size_t HtmlTemplateUProxiesSize;

// T_PROXIES_HEAD[1-12], T_PROXIES_TABLE_ITEMS_[START/END], T_CFG_TABLE_ODD, T_CFG_TABLE_EVEN, T_CFG_TABLE_OK, T_CFG_TABLE_WARN, T_CFG_TABLE_ERR, T_PROXIES_ITEM[1-12]
HTML_TEMPLATE_COMPONENT *HtmlTemplateProxies;
size_t HtmlTemplateProxiesSize;

// T_PRXSRC_HEAD[1-3], T_PRXSRC_TABLE_ITEMS_[START/END], T_CFG_TABLE_ODD, T_CFG_TABLE_EVEN, T_CFG_TABLE_OK, T_CFG_TABLE_WARN, T_CFG_TABLE_ERR, T_PRXSRC_ITEM[1-3]
HTML_TEMPLATE_COMPONENT *HtmlTemplateProxySources;
size_t HtmlTemplateProxySourcesSize;

// T_STATS_GEO_HEAD[1-2], T_STATS_GEO_TABLE_ITEMS_[START/END], T_STATS_GEO_ITEM[1-2]
HTML_TEMPLATE_COMPONENT *HtmlTemplateStats;
size_t HtmlTemplateStatsSize;

// T_CHECK_IP, T_CHECK_PORT, T_CHECK_TYPE, T_CHECK_COUNTRY
HTML_TEMPLATE_COMPONENT *HtmlTemplateCheck;
size_t HtmlTemplateCheckSize;

void HtmlTemplateParse(FILE *hFile, HTML_TEMPLATE_COMPONENT **Template, size_t *SizeRef, config_setting_t *CfgRoot);
void HtmlTemplateLoadAll();
void HtmlTemplateBufferInsert(struct evbuffer *Buffer, HTML_TEMPLATE_COMPONENT *Components, size_t Size, INTERFACE_INFO Info, HTML_TEMPALTE_TABLE_INFO TableInfo);
void HtmlTemplateMimeTypesInit();