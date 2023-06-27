#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "ncist.h"
#include "plug.h"

#include "backloggd.h"

PLUGIN_PREAMBLE("Backloggd")


int init()
{
    return 0;
}

int cmdparse(char *cmd)
{
    int rc;
    char *args;
    char buf[0x8000];
    pthread_t tid;

    if (CMD(cmd, "bl journal") || CMD(cmd, "bl j"))
	{
        callback("Journal not implemented yet");
        return 0;
	}
    else if (CMD(cmd, "bl find") || CMD(cmd, "bl f"))
	{
        if (pthread_create(&tid, NULL, bl_find, args) == 0)
        {
            callback("Searching for %s...", args);
        }
        return 0;
	}

    return -1;
}

int process()
{
    return 0;
}

int response_rx(void *contents, size_t size, size_t nmemb, resp_t *userp)
{
    size_t realsize = size * nmemb;

    memcpy(userp->buf + userp->size, contents, realsize);
    userp->size += realsize;
  
    return realsize;
}


int bl_journal()
{
    /*CURL *curl;
    CURLcode rc;
    char addr[256] = BL_URL "/autocomplete.js?query=";

    //strcat(addr, )

    curl = curl_easy_init();
    if (!curl) return -1;

    //curl_easy_setopt(curl, CURLOPT_)
    curl_easy_setopt(curl, CURLOPT_URL, BL_URL "/autocomplete.js");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_rx);

    rc = curl_easy_perform(curl);
    if (rc != CURLE_OK)
    {
        return -2;
    }

    curl_easy_cleanup(curl);
*/
    callback("Journal not implemented yet");
    return 0;
}

int bl_find(char *name)
{
    CURL *curl;
    CURLcode rc;
    resp_t resp = { .size = 0 };
    char url[512] = "https://backloggd.com/autocomplete.json?query=";

    curl = curl_easy_init();
    if (!curl) return -1;

    char *escaped = curl_easy_escape(curl, name, 0);
    strcat(url, escaped);
    curl_free(escaped);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_rx);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
    rc = curl_easy_perform(curl);

    if (rc != CURLE_OK)
    {
        callback("Curl error: %d", rc);
        return -2;
    }

    curl_easy_cleanup(curl);

    cJSON *json;
    cJSON *games, *tmp, *game;

    json = cJSON_Parse(resp.buf);
    if (!json)
    {
        callback("Response could not be parsed.");
        return -1;
    }

    games = cJSON_GetObjectItem(json, "suggestions");
    callback("Found %d games:", cJSON_GetArraySize(games));
    cJSON_ArrayForEach(game, games)
    {
        tmp = cJSON_GetObjectItem(game, "value");
        callback("  - %s", cJSON_GetStringValue(tmp));
    }

    cJSON_Delete(json);  

    return 0;
}