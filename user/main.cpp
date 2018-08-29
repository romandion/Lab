
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <UserEnv.h>
#include <errno.h>

int get_pw(char *& user_path , char *&user_name) ;

int main(int argc , char * argv[])
{

    
    
    return 0 ;
}


int get_pw(char *& user_path , char *&user_name) 
{
  HANDLE token;
  wchar_t username[256];
  wchar_t path[266];
  DWORD bufsize;
  int r;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &token) == 0)
    return -1 ;

  bufsize = sizeof(path);
  if (!GetUserProfileDirectoryW(token, path, &bufsize))
  {
    r = GetLastError();
    CloseHandle(token);

    if (r == ERROR_INSUFFICIENT_BUFFER)
      return ENOMEM;

    return -1 ;
  }

  CloseHandle(token);

  bufsize = sizeof(username);
  if(!GetUserNameW(username, &bufsize))
  {
    r = GetLastError();
    if (r == ERROR_INSUFFICIENT_BUFFER)
      return ENOMEM;

    return -1 ;
  }

  return 0;
}
