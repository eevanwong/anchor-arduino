#ifndef CACHE_H
#define CACHE_H

#include <Preferences.h>
#include <Arduino.h>

extern Preferences pref;

void cacheData(const char namespace, const char key, const any* data);
void retrieveData(const char namespace, const char key, any* data);
void clearCache(const char namespace, const char key);

#endif // CACHE_H