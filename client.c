#include <stdio.h>
#include <curl/curl.h>

// Callback function to handle the response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    printf("%.*s", (int)realsize, (char *)contents);
    return realsize;
}

int main()
{
    CURL *curl;
    CURLcode res;

    // Initialize the libcurl library
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create a curl handle
    curl = curl_easy_init();
    if (curl)
    {
        // Set the URL of the API endpoint
        curl_easy_setopt(curl, CURLOPT_URL, "https://random-data-api.com/api/v2/users");

        // Set the callback function to handle the response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_easy_cleanup(curl);
    }

    // Cleanup the libcurl library
    curl_global_cleanup();

    return 0;
}
