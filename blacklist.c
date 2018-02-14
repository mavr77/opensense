#include "opensense.h"

/*
  returns 0 if url not fount. or returns 1 if url in in blacklist
*/
int blacklist_logic(char *blacklist, char url[URL_SIZE])
{
  char line[URL_SIZE];
  char blacklist_url[URL_SIZE];
  FILE *file_desc = NULL;
  int line_num = 0;

  file_desc = fopen(blacklist, "r");
  if(file_desc == NULL){
    printf("%s %s\n", "could not open file", blacklist);
    return 0;
  }

  while(fgets(line, URL_SIZE, file_desc) != NULL){
    line_num++;
    if(line[0] == '#' || line[0] == '\n')
      continue;
    // sscanf function casting line which may have non printable characters to string for further comparison
    if(sscanf(line, "%s", blacklist_url) != 1)
      continue;

    if(strstr(url, blacklist_url) != NULL){
      fclose(file_desc);
      return 1;
    }
  }
  fclose(file_desc);
  return 0;
}
