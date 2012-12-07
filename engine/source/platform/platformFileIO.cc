//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "collection/vector.h"
#include "console/console.h"
#include "string/stringTable.h"
#include "io/resource/resourceManager.h"

//-----------------------------------------------------------------------------

StringTableEntry Platform::getTemporaryDirectory()
{
   StringTableEntry path = osGetTemporaryDirectory();

   if(! Platform::isDirectory(path))
      path = Platform::getCurrentDirectory();

   return path;
}

#ifdef TORQUE_TOOLS
ConsoleFunction(getTemporaryDirectory, const char *, 1, 1, "() Gets the path to the system's temporary directory\n"
                "@return Returns the path to the temporary directory as a string")
{
   return Platform::getTemporaryDirectory();
}
#endif

StringTableEntry Platform::getTemporaryFileName()
{
   char buf[512];
   StringTableEntry path = Platform::getTemporaryDirectory();

   dSprintf(buf, sizeof(buf), "%s/tgb.%08x.%02x.tmp", path, Platform::getRealMilliseconds(), U32(Platform::getRandom() * 255));

   // [tom, 9/7/2006] This shouldn't be needed, but just in case
   if(Platform::isFile(buf))
      return Platform::getTemporaryFileName();

   return StringTable->insert(buf);
}

#ifdef TORQUE_TOOLS
ConsoleFunction(getTemporaryFileName, const char *, 1, 1, "() Generates a temporary filename for use.\n"
                "@return Returns the formated temporary filename for use")
{
   return Platform::getTemporaryFileName();
}
#endif

//-----------------------------------------------------------------------------
static char filePathBuffer[1024];
static bool deleteDirectoryRecusrive( const char* pPath )
{
    // Sanity!
    AssertFatal( pPath != NULL, "Cannot delete directory that is NULL." );

    // Find directories.
    Vector<StringTableEntry> directories;
    if ( !Platform::dumpDirectories( pPath, directories, 0 ) )
    {
        // Warn.
        Con::warnf( "Could not retrieve sub-directories of '%s'.", pPath );
        return false;
    }

    // Iterate directories.
    for( Vector<StringTableEntry>::iterator basePathItr = directories.begin(); basePathItr != directories.end(); ++basePathItr )
    {
        // Fetch base path.
        StringTableEntry basePath = *basePathItr;

        // Skip if the base path.
        if ( basePathItr == directories.begin() && dStrcmp( pPath, basePath ) == 0 )
            continue;

        // Delete any directories recursively.
        if ( !deleteDirectoryRecusrive( basePath ) )
            return false;
    }

    // Find files.
    Vector<Platform::FileInfo> files;
    if ( !Platform::dumpPath( pPath, files, 0 ) )
    {
        // Warn.
        Con::warnf( "Could not retrieve files for directory '%s'.", pPath );
        return false;
    }

    // Iterate files.
    for ( Vector<Platform::FileInfo>::iterator fileItr = files.begin(); fileItr != files.end(); ++fileItr )
    {
        // Format file.
        dSprintf( filePathBuffer, sizeof(filePathBuffer), "%s/%s", fileItr->pFullPath, fileItr->pFileName );

        // Delete file.
        if ( !Platform::fileDelete( filePathBuffer ) )
        {
            // Warn.
            Con::warnf( "Could not delete file '%s'.", filePathBuffer );
            return false;
        }
    }

    // Delete the directory.
    if ( !Platform::fileDelete( pPath ) )
    {
        // Warn.
        Con::warnf( "Could not delete directory '%s'.", pPath );
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

bool Platform::deleteDirectory( const char* pPath )
{
    // Sanity!
    AssertFatal( pPath != NULL, "Cannot delete directory that is NULL." );

    // Is the path a file?
    if ( Platform::isFile( pPath ) )
    {
        // Yes, so warn.
        Con::warnf( "Cannot delete directory '%s' as it specifies a file.", pPath );
        return false;
    }

    // Expand module location.
    char pathBuffer[1024];
    Con::expandPath( pathBuffer, sizeof(pathBuffer), pPath, true );

    // Delete directory recursively.
    return deleteDirectoryRecusrive( pathBuffer );
}

//-----------------------------------------------------------------------------

static StringTableEntry sgMainCSDir = NULL;

StringTableEntry Platform::getMainDotCsDir()
{
   if(sgMainCSDir == NULL)
      sgMainCSDir = Platform::getExecutablePath();

   return sgMainCSDir;
}

void Platform::setMainDotCsDir(const char *dir)
{
   sgMainCSDir = StringTable->insert(dir);
}

//-----------------------------------------------------------------------------

typedef Vector<char*> CharVector;
static CharVector gPlatformDirectoryExcludeList;

void Platform::addExcludedDirectory(const char *pDir)
{
   gPlatformDirectoryExcludeList.push_back(dStrdup(pDir));
}

void Platform::clearExcludedDirectories()
{
   while(gPlatformDirectoryExcludeList.size())
   {
      dFree(gPlatformDirectoryExcludeList.last());
      gPlatformDirectoryExcludeList.pop_back();
   }
}

bool Platform::isExcludedDirectory(const char *pDir)
{
   for(CharVector::iterator i=gPlatformDirectoryExcludeList.begin(); i!=gPlatformDirectoryExcludeList.end(); i++)
      if(!dStricmp(pDir, *i))
         return true;

   return false;
}

//-----------------------------------------------------------------------------

inline void catPath(char *dst, const char *src, U32 len)
{
   if(*dst != '/')
   {
      ++dst; --len;
      *dst = '/';
   }

   ++dst; --len;

   dStrncpy(dst, src, len);
   dst[len - 1] = 0;
}

// converts the posix root path "/" to "c:/" for win32
// FIXME: this is not ideal. the c: drive is not guaranteed to exist.
#if defined(TORQUE_OS_WIN32)
static inline void _resolveLeadingSlash(char* buf, U32 size)
{
   if(buf[0] != '/')
      return;

   AssertFatal(dStrlen(buf) + 2 < size, "Expanded path would be too long");
   dMemmove(buf + 2, buf, dStrlen(buf));
   buf[0] = 'c';
   buf[1] = ':';
}
#endif

char * Platform::makeFullPathName(const char *path, char *buffer, U32 size, const char *cwd /* = NULL */)
{
   char bspath[1024];
   dStrncpy(bspath, path, sizeof(bspath));
   bspath[sizeof(bspath)-1] = 0;
   
   for(U32 i = 0;i < dStrlen(bspath);++i)
   {
      if(bspath[i] == '\\')
         bspath[i] = '/';
   }

   if(Platform::isFullPath(bspath))
   {
      // Already a full path
      #if defined(TORQUE_OS_WIN32)
         _resolveLeadingSlash(bspath, sizeof(bspath));
      #endif
      dStrncpy(buffer, bspath, size);
      buffer[size-1] = 0;
      return buffer;
   }

   if(cwd == NULL)
      cwd = Platform::getCurrentDirectory();

   dStrncpy(buffer, cwd, size);
   buffer[size-1] = 0;

   char *ptr = bspath;
   char *slash = NULL;
   char *endptr = buffer + dStrlen(buffer) - 1;

   do
   {
      slash = dStrchr(ptr, '/');
      if(slash)
      {
         *slash = 0;

         // Directory

         if(dStrcmp(ptr, "..") == 0)
         {
            // Parent
            endptr = dStrrchr(buffer, '/');
         }
         else if(dStrcmp(ptr, ".") == 0)
         {
            // Current dir
         }
         else if(endptr)
         {
            catPath(endptr, ptr, size - (endptr - buffer));
            endptr += dStrlen(endptr) - 1;
         }
         
         ptr = slash + 1;
      }
      else if(endptr)
      {
         // File

         catPath(endptr, ptr, size - (endptr - buffer));
         endptr += dStrlen(endptr) - 1;
      }

   } while(slash);

   return buffer;
}

bool Platform::isFullPath(const char *path)
{
   // Quick way out
   if(path[0] == '/' || path[1] == ':')
      return true;

   return false;
}

//-----------------------------------------------------------------------------

StringTableEntry Platform::makeRelativePathName(const char *path, const char *to)
{
   char buffer[1024];

   if(path[0] != '/' && path[1] != ':')
   {
      // It's already relative, bail
      return StringTable->insert(path);
   }

   // [tom, 12/13/2006] We need a trailing / for this to work, so add one if needed
   if(*(to + dStrlen(to) - 1) != '/')
   {
      dSprintf(buffer, sizeof(buffer), "%s/", to);
      to = StringTable->insert(buffer);
   }

   const char *pathPtr, *toPtr, *branch = path;
   char *bufPtr = buffer;

   // Find common part of path
   for(pathPtr = path, toPtr = to;*pathPtr && *toPtr && dTolower(*pathPtr) == dTolower(*toPtr);++pathPtr, ++toPtr)
   {
      if(*pathPtr == '/')
         branch = pathPtr;
   }

   if((*pathPtr == 0 || (*pathPtr == '/' && *(pathPtr + 1) == 0)) &&
      (*toPtr == 0 || (*toPtr == '/' && *(toPtr + 1) == 0)))
   {
      *bufPtr++ = '.';

      if(*pathPtr == '/' || *(pathPtr - 1) == '/')
         *bufPtr++ = '/';

      *bufPtr = 0;
      return StringTable->insert(buffer);
   }

   if((*pathPtr == 0 && *toPtr == '/') || (*toPtr == '/' && *pathPtr == 0))
      branch = pathPtr;

   // Figure out parent dirs
   for(toPtr = to + (branch - path);*toPtr;++toPtr)
   {
      if(*toPtr == '/' && *(toPtr + 1) != 0)
      {
         *bufPtr++ = '.';
         *bufPtr++ = '.';
         *bufPtr++ = '/';
      }
   }
   *bufPtr = 0;

   // Copy the rest
   if(*branch)
      dStrcpy(bufPtr, branch + 1);
   else
      *--bufPtr = 0;

   return StringTable->insert(buffer);
}

//-----------------------------------------------------------------------------

static StringTableEntry tryStripBasePath(const char *path, const char *base)
{
   U32 len = dStrlen(base);
   if(dStrnicmp(path, base, len) == 0)
   {
      if(*(path + len) == '/') ++len;
      return StringTable->insert(path + len, true);
   }
   return NULL;
}

StringTableEntry Platform::stripBasePath(const char *path)
{
   StringTableEntry str = NULL;
    
   str = tryStripBasePath( path, Platform::getMainDotCsDir() );
    
   if ( str )
      return str;
    
   str = tryStripBasePath( path, Platform::getCurrentDirectory() );

   if ( str )
      return str;
    
   str = tryStripBasePath( path, Platform::getPrefsPath() );

   if ( str )
      return str;

    
   return path;
}

//-----------------------------------------------------------------------------

StringTableEntry Platform::getPrefsPath(const char *file /* = NULL */)
{
    char buf[1024];
#ifdef TORQUE_OS_IOS
    
    if ( file )
    {
        if ( dStrstr( file, ".." ) || dStrstr( file, "./" ) || dStrstr( file, "~/" ) )
        {
            Con::errorf( "getPrefsPath - Filename (%s) cannot be relative.", file );
            
            return NULL;
        }
        
        dSprintf( buf, sizeof( buf ), "%s/%s", Platform::getUserDataDirectory(), file );
    }
    else
    {
        dSprintf( buf, sizeof( buf ), "%s", Platform::getUserDataDirectory() );
    }
    
    return StringTable->insert(buf, true);

#endif

   const char *company = Con::getVariable("$Game::CompanyName");
   if(company == NULL || *company == 0)
      company = "GarageGames";

   const char *appName = Con::getVariable("$Game::ProductName");
   if(appName == NULL || *appName == 0)
      appName = TORQUE_GAME_NAME;

   if(file)
   {
      if(dStrstr(file, "..") || dStrstr(file, "./") || dStrstr(file, "~/"))
      {
         Con::errorf("getPrefsPath - Filename (%s) cannot be relative.", file);
         return NULL;
      }

      dSprintf(buf, sizeof(buf), "%s/%s/%s/%s", Platform::getUserDataDirectory(), company, appName, file);
   }
   else
      dSprintf(buf, sizeof(buf), "%s/%s/%s", Platform::getUserDataDirectory(), company, appName);

   return StringTable->insert(buf, true);
}

//-----------------------------------------------------------------------------

#ifdef TORQUE_TOOLS
ConsoleFunction(getUserDataDirectory, const char*, 1, 1, "()\n"
                "@return Returns a string to the directory storing the user's data")
{
   return Platform::getUserDataDirectory();
}

ConsoleFunction(getUserHomeDirectory, const char*, 1, 1, "() \n"
                "@return Returns the path to the user's home directory.")
{
   return Platform::getUserHomeDirectory();
}
#endif
