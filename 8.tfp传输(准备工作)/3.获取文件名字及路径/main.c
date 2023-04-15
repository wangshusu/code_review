#include "getpath.h"
#include <unistd.h>

int main(int argc,char *argv[])
{
    while(1)
    {
        sleep(2);

        Queue *qu_path = get_dir_path("./");
        getpath_Print(qu_filename);
        getpath_Print(qu_path);
        
        queue_Destroy(&qu_filename,NULL);
        queue_Destroy(&qu_path,NULL);
    
    }
	
    return 0;
}