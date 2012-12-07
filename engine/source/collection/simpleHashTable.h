//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// [tom, 9/19/2006] Simple hash table. Not intended to replace map<>, but it is
// generally good enough for simple things that you don't need to iterate.
//
// Note: If you move this to another project, you need the updated tSparseArray.h
// as well as hashFunction.cc/h

#include "platform/platform.h"

#include "sparseArray.h"
#include "algorithm/hashFunction.h"

#ifndef _TSIMPLEHASHTABLE_H
#define _TSIMPLEHASHTABLE_H

template <class T> class SimpleHashTable : public SparseArray<T>
{
   typedef SparseArray<T> Parent;

   bool mCaseSensitive;

   char mCaseConvBuf[1024];
   
   // [tom, 9/21/2006] This is incredibly lame and adds a pretty big speed penalty
   inline const char *caseConv(const char *str)
   {
      if(mCaseSensitive)   return str;

      S32 len = dStrlen(str);
      if(len >= sizeof(mCaseConvBuf))  len = sizeof(mCaseConvBuf) - 1;

      char *dptr = mCaseConvBuf;
      const char *sptr = str;
      while(*sptr)
      {
         *dptr = dTolower(*sptr);
         ++sptr;
         ++dptr;
      }
      *dptr = 0;

      return mCaseConvBuf;
   }

public:
   SimpleHashTable(const U32 modulusSize = 64, bool caseSensitive = true) : Parent(modulusSize), mCaseSensitive(caseSensitive)
   {
   }

   void insert(T* pObject, U8 *key, U32 keyLen);
   T*   remove(U8 *key, U32 keyLen);
   T*   retrieve(U8 *key, U32 keyLen);

   void insert(T* pObject, const char *key);
   T*   remove(const char *key);
   T*   retrieve(const char *key);
};

template <class T> inline void SimpleHashTable<T>::insert(T* pObject, U8 *key, U32 keyLen)
{
   Parent::insert(pObject, hash(key, keyLen, 0));
}

template <class T> inline T* SimpleHashTable<T>::remove(U8 *key, U32 keyLen)
{
   return Parent::remove(hash(key, keyLen, 0));
}

template <class T> inline T* SimpleHashTable<T>::retrieve(U8 *key, U32 keyLen)
{
   return Parent::retrieve(hash(key, keyLen, 0));
}

template <class T> inline void SimpleHashTable<T>::insert(T* pObject, const char *key)
{
   key = caseConv(key);
   insert(pObject, (U8 *)key, dStrlen(key));
}

template <class T> T* SimpleHashTable<T>::remove(const char *key)
{
   key = caseConv(key);
   return remove((U8 *)key, dStrlen(key));
}

template <class T> T* SimpleHashTable<T>::retrieve(const char *key)
{
   key = caseConv(key);
   return retrieve((U8 *)key, dStrlen(key));
}


#endif // _TSIMPLEHASHTABLE_H
