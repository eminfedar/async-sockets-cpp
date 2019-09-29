#ifdef BUILD_EASYSOCKET
#define EASYSOCKET_API EXPORT_EASYSOCKET
#else
#define EASYSOCKET_API IMPORT_EASYSOCKET
#endif

#if defined(_MSC_VER)
    //  Microsoft 
    #define EXPORT_EASYSOCKET __declspec(dllexport)
    #define IMPORT_EASYSOCKET __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define EXPORT_EASYSOCKET __attribute__((visibility("default")))
    #define IMPORT_EASYSOCKET
#else
    //  do nothing and hope for the best?
    #define EXPORT_EASYSOCKET
    #define IMPORT_EASYSOCKET
    #pragma warning Unknown dynamic link import/export semantics.
#endif
