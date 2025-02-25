#include <cache_handler.h>

Preferences pref;

void cacheData(const char namespace, const char key, const any* data) {
    pref.begin(namespace, false);
    pref.putBytes(key, data, sizeof(data)+1);
    pref.end();
}

void retrieveData(const char namespace, const char key, any* data) {
    pref.begin(namespace, true);
    size_t len = pref.getBytesLength(key);
    if (len > 0) {
        data = (any*)malloc(len);
        pref.getBytes(key, data, len);
        data[len] = '\0';
    }
    pref.end();
}

void clearCache(const char namespace, const char key) {
    pref.begin(namespace, false);
    pref.remove(key);
    pref.end();
}
