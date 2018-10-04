#ifndef CONFIG_h
#define CONFIG_h

#define SN "multisensor " NODE_ID
#define SV "2.0"

#ifdef DEVELOPMENT
#define NODE_ID "esp12f"
#undef DISABLE_LOGGING
#else
#define DISABLE_LOGGING
#undef MY_DEBUG
#endif

#define MAX_SENSOR_ID 6

#endif
