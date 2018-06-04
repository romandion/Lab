
#ifndef __INTRIN_DLL_H
#define __INTRIN_DLL_H 1


#ifndef INTRIN_IMPORT
#define INTRIN_IMPORT   __declspec(dllimport)
#endif

#ifndef INTRIN_EXPORT
#define INTRIN_EXPORT   __declspec(dllexport)
#endif


#ifdef INTRIN_DLL_EXPORTS
	#define INTRINAPI INTRIN_EXPORT
#else
    #define INTRINAPI INTRIN_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

INTRINAPI int __builtin_ffs (int x)  ;

#ifdef __cplusplus
}
#endif



#endif /** __INTRIN_DLL_H */

