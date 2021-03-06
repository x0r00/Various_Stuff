/*
 *     ____             _________                __                _     
 *    / __ \___  ____ _/ /_  __(_)___ ___  ___  / /   ____  ____ _(_)____
 *   / /_/ / _ \/ __ `/ / / / / / __ `__ \/ _ \/ /   / __ \/ __ `/ / ___/
 *  / _, _/  __/ /_/ / / / / / / / / / / /  __/ /___/ /_/ / /_/ / / /__  
 * /_/ |_|\___/\__,_/_/ /_/ /_/_/ /_/ /_/\___/_____/\____/\__, /_/\___/  
 *                                                       /____/          
 *
 *                  Barracuda Embedded Web-Server 
 ****************************************************************************
 *            HEADER
 *
 *   $Id: BufPrint.h 3459 2014-08-18 22:38:07Z wini $
 *
 *   COPYRIGHT:  Real Time Logic LLC, 2008
 *
 *   This software is copyrighted by and is the sole property of Real
 *   Time Logic LLC.  All rights, title, ownership, or other interests in
 *   the software remain the property of Real Time Logic LLC.  This
 *   software may only be used in accordance with the terms and
 *   conditions stipulated in the corresponding license agreement under
 *   which the software has been supplied.  Any unauthorized use,
 *   duplication, transmission, distribution, or disclosure of this
 *   software is expressly forbidden.
 *                                                                        
 *   This Copyright notice may not be removed or modified without prior
 *   written consent of Real Time Logic LLC.
 *                                                                         
 *   Real Time Logic LLC. reserves the right to modify this software
 *   without notice.
 *
 *               http://www.realtimelogic.com
 ****************************************************************************
 *
 */

#ifndef __BufPrint_h
#define __BufPrint_h

#ifdef __cplusplus
#ifndef putc
#define putc noputc
#include <stdio.h>
#endif
#undef putc
#endif

#include "BaAtoi.h"
#include <stdarg.h>
#include <string.h>

struct BufPrint;

#ifdef __cplusplus
extern "C" {
#endif

/** This function is similar to sprintf, but uses much less stack and
    is more suitable for a real time operating system with limited
    stack size.

    \param buf The buffer to where the data is formatted.
    \param fmt See BufPrint::printf
*/
BA_API int basprintf(char* buf, const char* fmt, ...);

/** A safe version of sprintf.  This function is similar to
    snprintf. The second argument specifies the buffer length and
    basnprintf returns -1 if the buffer being formatted should exceed
    that length.

    \param buf The buffer to where the data is formatted.
    \param len length of the buffer.
    \param fmt See BufPrint::printf
*/
BA_API int basnprintf(char* buf, int len, const char* fmt, ...);

#ifdef __cplusplus
#undef printf
}
#endif 

/** @defgroup BufPrint printf compatible function
   @{

\sa <a href="../../index.html" _target="top"> Barracuda Introduction </a>
 */

/** BufPrint flush callback function.

A BufPrint instance calls the flush callback function when buffer is
full. The callback can either extend the buffer or flush and reset the
buffer.

\param o the object. BufPrint is typically upcasted to the derived object.
\param sizeRequired the required expands size if the callback is
expanding the buffer. Not used when flushing and resetting the buffer.
*/
typedef int (*BufPrint_Flush)(struct BufPrint* o, int sizeRequired);

/** The BufPrint class, which implements an ANSI compatible printf
    method, is an abstract class used as a base for many of the
    Barracuda classes.

    The output from printf is formatted in an internal buffer. This
    class does not allocate memory for the buffer. Thus, any class
    using BufPrint must provide a buffer BufPrint can use. BufPrint
    calls the callback function BufPrint_Flush when the buffer is
    full.
 */
typedef struct BufPrint
{
#ifdef __cplusplus

      /** BufPrint constructor.

          \param userData an optional argument stored in the BufPrint
          object and accessible in the flush callback.
          \sa getUserData()

          \param flush a pointer to the flush callback function.
      */
      //BufPrint(void* userData=0, BufPrint_Flush flush=0);
      BufPrint()
        {}

      /** Returns the user data pointer set in the constructor.
       */
      void* getUserData();

      void init(void* userData=0, BufPrint_Flush flush=0);
      
      /** The printf function's format is identical to the standard
          ANSI vprintf function. See BufPrint::printf for the format flags.
          \param fmt See vprintf in the
<a href="http://www.infosys.utas.edu.au/info/documentation/C/CStdLib.html#stdio.h">
          C Standard Library</a> for more information.
          \param argList See vprintf in the C Standard Library for
          more information.
      */
      int vprintf(const char* fmt, va_list argList);

      /** The printf function's format is identical to the standard
          ANSI printf function, but with the following extensions:

          \li %%lld is for printing S64 or a signed representation of a U64.
          \li %%llu is for printing an unsigned U64.
          \li %%j is similar to %%s, but the string is encoded by using
          BufPrint::jsonString

          \param fmt See vprintf in the
          <a href="../../../StdC/CStdLib.html#stdio.h">
          C Standard Library</a> for more information.
       */
      int printf(const char* fmt, ...);

      /** print character */
      int putc(int c);

      /** Write data to the buffer.
       * \param data pointer to data.
       * \param len size of data.
       */
      int write(const void* data, int len);

      /** Used for sending a zero terminated string to the client.
         C method name is BufPrint_write2.
         \param buf a reference to the string.
       */
      int write(const char* buf);

      void setFlush(BufPrint_Flush flush);
      void setUserData(void* userData);
      
   /** Set the buffer used by BufPrint
    \param buf the buffer
    \param size the buffer size
   */
   void setBuf(char* buf, int size);


      /** Returns a pointer to the internal buffer. Please note that
       * the buffer returned by this method is not zero terminated.
       * \sa BufPrint::getBufSize
       */
      char* getBuf();

      /** Returns current size of internal formatted data */
      U32 getBufSize();


      /** resets the cursor, thus erasing the data in the buffer */
      void erase();

      /** Flush buffer */
      int flush();

      /** Encode binary data as B64.
          \sa baB64Decode
          \param data binary data or string to be encoded as B64.
          \param slen the data size.
      */
      int b64Encode(const void* data, S32 slen);


      /** Print and escape a string such that a browser can run the
          JavaScript 'eval' function and produce a string identical to the
          string the 'str' argument points to. The string must be ASCII or
          UTF8. A UTF8 string is converted to JavaScript Unicode i.e. to
          \\uxxxx.

          The function can, for example, be used if the server generates a
          <a href="http://json.org">JSON</a>
          response or generates dynamic JavaScript in a CSP page.
          \sa BufPrint::printf with format flag j
      */
      int jsonString(const char* str);
#endif
      BufPrint_Flush flushCB;
      void *userData;
      char* buf;
      int cursor;
      int bufSize;
}BufPrint;

#define BufPrint_putcMacro(o, c) do { \
   if((o)->cursor == (o)->bufSize) \
   { \
      if((o)->flushCB(o, 1)) \
         return -1; \
   } \
   (o)->buf[(o)->cursor++] = c; \
} while(0)

#ifdef __cplusplus
extern "C" {
#endif
#define BufPrint_getUserData(o) (o)->userData
#define BufPrint_erase(o) (o)->cursor=0
#define BufPrint_getBuf(o) (o)->buf
#define BufPrint_setBuf(o, b, size) (o)->buf=b,(o)->bufSize=size,(o)->cursor=0
#define BufPrint_getBufSize(o) (o)->cursor
BA_API void BufPrint_constructor(
   BufPrint* o,void* userData,BufPrint_Flush flush);
#define BufPrint_destructor(o)
BA_API int BufPrint_vprintf(BufPrint* o, const char* fmt, va_list argList);
BA_API int BufPrint_printf(BufPrint* o, const char* fmt, ...);
BA_API int BufPrint_write(BufPrint* o, const void* data, int len);
BA_API int BufPrint_putc(BufPrint* o, int c);
BA_API int BufPrint_flush(BufPrint* o);
#define BufPrint_write2(o, data) BufPrint_write(o, data, -1)
BA_API int BufPrint_b64Encode(BufPrint* o, const void* source, S32 slen);
BA_API int BufPrint_jsonString(BufPrint* o, const char* str);
#ifdef __cplusplus
}
inline void* BufPrint::getUserData() {
   return BufPrint_getUserData(this);
}
//inline BufPrint::BufPrint(void* userData, BufPrint_Flush flush) {
//   BufPrint_constructor(this, userData,flush); }
inline void BufPrint::init(void* userData, BufPrint_Flush flush) {
   BufPrint_constructor(this, userData, flush); }
inline int BufPrint::vprintf(const char* fmt, va_list argList) {
   return BufPrint_vprintf(this, fmt, argList); }
inline int BufPrint::printf(const char* fmt, ...) {
   int retv; va_list varg;
   va_start(varg, fmt);
   retv = BufPrint_vprintf(this, fmt, varg);
   va_end(varg);
   return retv;
}
inline char* BufPrint::getBuf() {
   return BufPrint_getBuf(this); }
inline void BufPrint::setBuf(char* buf, int size) {
   BufPrint_setBuf(this, buf, size); }
inline U32 BufPrint::getBufSize() {
   return BufPrint_getBufSize(this); }
inline void BufPrint::erase() {
   BufPrint_erase(this); }
inline int BufPrint::putc(int c) {
   return BufPrint_putc(this, c); }
inline int BufPrint::write(const void* data, int len) {
   return BufPrint_write(this, data, len); }
inline int BufPrint::write(const char* data) {
   return BufPrint_write2(this, data); }
inline void BufPrint::setFlush(BufPrint_Flush flush) {
   this->flushCB = flush;
}
inline void BufPrint::setUserData(void* userData) {
   this->userData = userData;
}
inline int BufPrint::flush() {
   return BufPrint_flush(this);
}
inline int BufPrint::b64Encode(const void* source, S32 slen){
   return BufPrint_b64Encode(this, source, slen);
}
inline int BufPrint::jsonString(const char* str){
   return BufPrint_jsonString(this, str);
}
#endif

/** @} */ /* end of BufPrint */


#endif
