/*mycopy.c - a third version of mycopy to copy file from dir to another dir
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *,char *);
void copyfile(char *,char *);
void copydir(char *,char *);

main( int ac,char *av[])
{
/*主函数，获取两个地址参数，如果直接为文件即 末尾不是‘/’，那就直接对文件进行复制
 *如果末尾是‘/’，则说明地址是文件夹， 调用copyfile函数对整个文件夹的所有内容复制
 */
if (ac != 3){
	fprintf(stderr,"usage : %s source destionation\n", *av);
	exit(1);
}
int i = 0;
int in_len = strlen(av[1]);
if(av[1][in_len-1] != '/')
	copyfile(av[1],av[2]);
else
	copydir(av[1],av[2]);
}

/*对单个文件进行复制*/
void copyfile(char *s1,char *s2)
{
int in_fd,out_fd,n_chars;
int i,tempi,tempo;
int in_len,out_len,filename_len,s3_len;
in_len = strlen(s1);	
out_len = strlen(s2);
char buf[BUFFERSIZE],s3[BUFFERSIZE];
/*get the filename*/
tempi = 0;
for( i = 0; i < in_len; i++){
	filename_len++;
	if(s1[i] == '/')	//得到文件名长度和文件名
		filename_len = 0;
}
tempi = in_len-filename_len;
tempo = out_len;
/*create the new file dir & name */
for(i = 0; i < out_len; i++)
	s3[i] = s2[i];
/*只对复制文件夹起作用，如果直接复制文件filename_len为0，将直接复制
 *如果是复制文件夹，filename_len即为当前文件夹下需要复制那个文件的文件名
 *此时将该文件名和目的地址合并成新的地址
 */
for(i = 0; i < filename_len; i++){
	s3[tempo] = s1[tempi];	//将获取的文件名和目的地址合并
	tempo++;
	tempi++;
}
s3[tempo] = '\0';


if(( in_fd = open (s1,O_RDONLY)) == -1)
	oops("Cannot open",s1);
if(( out_fd = creat (s3,COPYMODE)) == -1)
	oops("Cannot creat",s3);
while((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0 )
	if( write(out_fd,buf,n_chars)!=n_chars)
		oops("Write error to",s3);
if(n_chars == -1)
	oops("Read error from",s1);
if(close(in_fd) == -1 || close(out_fd) == -1)
	oops("Error closing files","");
}



void copydir(char *av1,char *av2)
{
DIR *dir_ptr;
struct dirent *direntp;

printf("copydir!!!\n");
if ((dir_ptr = opendir(av1)) == NULL)
	fprintf(stderr,"ls1:cannot open %s\n",av1);
else
{
/*从direntp中获得dirent结构，可以读出当前地址下的所有文件名称资料
 */
	while(( direntp = readdir(dir_ptr))!= NULL){
		printf("%s\n",direntp->d_name);
		//printf("sizeof d_name is %d\n",strlen(direntp->d_name));
		if(direntp->d_name[0] != '.'){
			char tempdir[50];
			strcpy(tempdir,av1);
			strcat(tempdir,direntp->d_name);
			//printf("%s\n",tempdir);
			copyfile(tempdir,av2);
		}
	}
	closedir(dir_ptr);
}
}


/*显示错误*/
void oops(char *s1,char *s2)
{
fprintf(stderr,"Error: %s",s1);
perror(s2);
exit(1);
}

