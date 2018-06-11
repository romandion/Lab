

#ifndef __MDLL_H
#define __MDLL_H 1

#ifdef MDLL_EXPORTS
	#define MDLLAPI __declspec(dllexport)
#else
    #define MDLLAPI __declspec(dllimport)
#endif

#pragma push_macro("malloc")
#pragma push_macro("free")

#undef malloc
#undef free


#define malloc(size)  ts_malloc(size)
#define free(addr)    ts_free(addr)


#ifdef __cplusplus
extern "C"{
#endif

MDLLAPI void * ts_malloc(size_t size) ;
MDLLAPI  void ts_free(void * addr) ;

#ifdef __cplusplus
}
#endif

#endif  /** __MDLL_H*/

