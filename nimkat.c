#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
struct stat b;
char t[ 100 ] = "";
char branch_name[50]="master";
char final_output[2000];
int new_addings=0;
char store_numbers[20][1024];
char*  last_modified_check(char address[])
{
     stat(address, &b);
     strftime(t, 100, "%d/%m/%Y %H:%M:%S", localtime(&b.st_mtime));
     return t;
}

char* existance(char dirname[])
{
    char where[2000];
    char checker[2000];
    char while_breaker[2000];
    
    struct dirent* moving;
    getcwd(where,sizeof(where));
    while(1)
    {
     DIR* directory=opendir(".");
     while((moving=readdir(directory))!=NULL)
     {
        if((moving->d_type==DT_DIR)&&(strcmp(dirname,moving->d_name)==0))
        {
            closedir(directory);
            chdir(dirname);
            getcwd(final_output,sizeof(final_output));
            chdir(where);
            return final_output;
        }
     } 
     closedir(directory);
     getcwd(checker,sizeof(checker));
     chdir("..");
     getcwd(while_breaker,sizeof(while_breaker));
     if(strcmp(while_breaker,checker)==0) break;
    };
    chdir(where);
    return NULL;
}

int run_config(int argc,char* argv[])
{
     if(strcmp(argv[2],"-global")==0)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        if(strcmp(argv[3],"user.name")==0)
        {
            FILE* global_name= fopen("G:\\bp_project\\globals\\config\\name.txt","w");
            for(int i=4;i<argc;i++) fprintf(global_name,"%s ",argv[i]);
            fclose(global_name);
            FILE* projects=fopen("G:\\bp_project\\globals\\projects.txt","r");
            char address[1024];
            while(!feof(projects))
            {
                fgets(address,1024,projects);
                address[strlen(address)-1]='\0';
                chdir(address);
                FILE* local_name=fopen("config\\name.txt","w");
                for(int i=4;i<argc;i++) fprintf(local_name,"%s ",argv[i]);      
                fclose(local_name);    
            }
            fclose(projects);
            chdir(cwd);
        }
        else if(strcmp(argv[3],"user.email")==0)
        {
            FILE* global_email= fopen("G:\\bp_project\\globals\\config\\email.txt","w");
            for(int i=4;i<argc;i++) fprintf(global_email,"%s ",argv[i]);
           fclose(global_email);
            FILE* projects=fopen("G:\\bp_project\\globals\\projects.txt","r");
            char address[1024];
            while(!feof(projects))
            {
                fgets(address,1024,projects);
                address[strlen(address)-1]='\0';
                chdir(address);
                FILE* local_email=fopen("config\\email.txt","w");
                for(int i=4;i<argc;i++) fprintf(local_email,"%s ",argv[i]);      
                fclose(local_email);
            }
                fclose(projects);
                chdir(cwd);    
        }
    }
    else if(strncmp(argv[2],"user",4)==0)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        if(strcmp(argv[2],"user.name")==0)
        {
            if(existance(".nimkat")==NULL) {printf("you have not initialized yet ");return 1;}
            else{
                chdir(existance(".nimkat"));
                FILE* local_name=fopen("config\\name.txt","w");
                for(int i=3;i<argc;i++) fprintf(local_name,"%s ",argv[i]);      
                fclose(local_name);
                chdir(cwd);
            }    
        }
        else if(strcmp(argv[2],"user.email")==0)
        {
            if(existance(".nimkat")==NULL) {printf("you have not initialized yet ");return 1;}
            else{
                chdir(existance(".nimkat"));
                FILE* local_email=fopen("config\\email.txt","w");
                for(int i=3;i<argc;i++) fprintf(local_email,"%s ",argv[i]);      
                fclose(local_email);
                chdir(cwd);
            }    
        }
    }
}

void branch_folder_maker(char branch_name[])
{
    char where[1024];
    getcwd(where,sizeof(where));
    chdir(existance(".nimkat"));
    mkdir(branch_name);
    chdir(branch_name);
    FILE* stat=fopen("status.txt","w");
    fclose(stat);
    FILE* l_stage=fopen("last_stages.txt","w");
    fclose(l_stage);
    //FILE* sf=fopen("staged_folders","w");
    //fclose(sf);
    //chdir(where);
}

int run_init(int argc,char* argv[])
{
    if(existance(".nimkat")!=NULL) {printf("already initialized!");}
    else
    {
     mkdir(".nimkat");
     mkdir(".nimkat\\config");
     branch_folder_maker("master");
   FILE* file=fopen("G:\\bp_project\\globals\\projects.txt","a");
   fprintf(file,"%s\n",existance(".nimkat"));
   fclose(file);
    }
}

int run_alias(int argc,char* argv[])
{
    if(strncmp(argv[3],"alias.",6)==0)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        FILE* projects=fopen("G:\\bp_project\\globals\\projects.txt","r");
        char address[1024];
        while(1)
        {
            fgets(address,1024,projects);
            if(feof(projects)) break;
            address[strlen(address)-1]='\0';
            chdir(address);
            FILE* alias=fopen("config\\aliases.txt","a");
            for(int i=6;i<=strlen(argv[3])-1;i++)
            {
                fprintf(alias,"%c",argv[3][i]);
            }
            fprintf(alias,"\n");
            fprintf(alias,"%s\n",argv[4]);
            fprintf(alias,"------------\n");
            fclose(alias);
        }
        fclose(projects);
    }
    else if(strncmp(argv[2],"alias.",6)==0)
    {
        if(existance(".nimkat")==NULL) {printf("You have not initialized yet!");return 1;}
       FILE* alias=fopen(strcat(existance(".nimkat"),"\\config\\aliases.txt"),"a");
       for(int i=6;i<=strlen(argv[2])-1;i++)
            {
                fprintf(alias,"%c",argv[2][i]);
            }
            fprintf(alias,"\n");
            fprintf(alias,"%s\n",argv[3]);
            fprintf(alias,"------------\n");
            fclose(alias);
        }
    }

int run_add(char address[])
{
   int flag=0;
    if(existance(".nimkat")==NULL) {printf("you have not initialized yet!");return 1;}
    if(fopen(address,"r")==NULL) {printf("the file failed to find!\n");return 1;}
    char time[100];
    char cwd[1024];
    char address_checker[1024];
    getcwd(cwd,sizeof(cwd));
    strcpy(time,last_modified_check(address));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    FILE* add=fopen("status.txt","r+");
    while(1)
    {
        fgets(address_checker,1024,add);
        address_checker[strlen(address_checker)-1]='\0';
        if(feof(add)) break;
        if(strcmp(address,address_checker)==0)
        { 
            flag=1;
            char written_time[1024];
            char mode[3];
            int a=ftell(add);
            fgets(written_time,1024,add);
            written_time[strlen(written_time)-1]='\0';
            fgets(mode,3,add);

            if((strcmp(written_time,time)==0)&&(strcmp(mode,"s\n")==0))
            {
            printf("the file is already added!");chdir(cwd);return 1;
            }
            else
            {
                FILE* addd=fopen("status.txt","r+");
                fseek(addd,a,SEEK_SET);
                fprintf(addd,"%s\n",time);
                fprintf(addd,"s\n");
                fclose(addd);
                new_addings++;
                strcpy(store_numbers[new_addings],address);  
            }
        }
    }
    fclose(add);
    if(flag==0)
    {
        FILE* new_add=fopen("status.txt","a");
        fprintf(new_add,"%s\n",address);
        fprintf(new_add,"%s\n",time);
        fprintf(new_add,"s\n");
        fclose(new_add);
        new_addings++;
        strcpy(store_numbers[new_addings],address);
    }
    chdir(cwd); 
}

int  general_add(char address[])
{
    if(existance(".nimkat")==NULL) {printf("you have not initialized yet");return 1;}
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    if((fopen(address,"r")==NULL)&&(chdir(address)!=0)) {printf("the address faild to find\n");return 1;}
    if(fopen(address,"r")!=NULL)
    {
        return run_add(address);
    }
    else if(chdir(address)==0)
    {
        struct dirent* entry;
        chdir(address);
        if(existance(".nimkat")==NULL) printf("you heve not initialized yet");
        DIR* dir=opendir(address);
        while ((entry = readdir(dir)) != NULL)
        {
            char file_address[1024];
            if(entry->d_type!=DT_DIR)
            {
                strcpy(file_address,address);
                strcat(file_address,"\\");
                strcat(file_address,entry->d_name);
                run_add(file_address);
            }
        }
    }
    chdir(cwd);
}

int is_staged(char address[],short int m)
{
    int flag=0;
    char cwd[1024];
    char time[200];
    strcpy(time,last_modified_check(address));
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    char address_checker[1024];
    FILE* status=fopen("status.txt","r+");
    if(status==NULL) {printf("faild to open status.txt,please contact us!");return -3;}
        while(1)
        {
            fgets(address_checker,1024,status);
            address_checker[strlen(address_checker)-1]='\0';
            if(feof(status)) break;
            if(strcmp(address,address_checker)==0)
            {
                flag=1;
                char mode[3];
                char written_time[200];
                fgets(written_time,200,status);
                written_time[strlen(written_time)-1]='\0';
                int c=ftell(status);
                if(m==1)
                {
                    if(fopen(address,"r")==NULL) return -1;
                    fclose(fopen(address,"r"));
                    fgets(mode,3,status);
                    if((strcmp(time,written_time)==0)&&(strcmp(mode,"s\n"))==0) return 1;
                    return 0;
                }
                if(m==0)
                {
                    fopen("status.txt","r+");
                    fseek(status,c,SEEK_SET);
                    fprintf(status,"u\n");
                    fclose(status);
                    return 2;
                }
            }
        }
    fclose(status);
    if(flag==0) 
    {
        if(m==1)
        {
            if(fopen(address,"r")==NULL) return -2;
            return -3;
        } 

        else if(m==0){printf("the file was not staged before!\n");return -4;}
     }
    chdir(cwd);
}

int staged_folder(char address[],short int m)
{
    struct dirent* entry;
    DIR* dir=opendir(address);
    if(dir==NULL) return -1;
    while((entry=readdir(dir))!=NULL)
    {
        if(entry->d_type!=DT_DIR)
        {
            char final_address[1024];
            strcpy(final_address,address);
            strcat(final_address,"\\");
            strcat(final_address,entry->d_name);
            int a =is_staged(final_address,m);
            if((a!=1)&&(m==1)) return 0; 
        }
    }
    closedir(dir);
   if(m==1) return 1;
}

int run_status(char address[])
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    struct dirent* entry;
    DIR* dir=opendir(".");
    while((entry=readdir(dir))!=NULL)
    {
        if(entry->d_type!=DT_DIR)
        {
            char final_address[1024];
            strcpy(final_address,cwd);
            strcat(final_address,"\\");
            strcat(final_address,entry->d_name);
            int a=is_staged(final_address,1);
            if(a==-1) {printf("%s -D\n",final_address);return 0;}
            else if(a==0){printf("%s -M",address);}
        }
    }
    closedir(dir);
}

int main(int argc,char* argv[])
{
if(argc<=1) {printf("just my name is not enough!");return 1;}

else if(strcmp(argv[1],"init")==0)
{
  return run_init(argc,argv);
}
else if((strcmp(argv[1],"config")==0))
{
    if((strncmp(argv[2],"user",4)==0)||(strncmp(argv[3],"user",4)==0))
    {
        return run_config(argc,argv);
    }
    else
    {
   int n;
    int flag=0;
    if(strncmp(argv[2],"alias.",6)==0) n=3;
    if(strncmp(argv[3],"alias.",6)==0) n=4;

   if(strncmp(argv[n],"nimkat init",strlen("nimkat init"))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config -global user.name",strlen("nimkat config -global user.name"))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config -global user.email",strlen("nimkat config -global user.email"))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config user.name",strlen("nimkat config user.name"))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config user.email",strlen("nimkat config user.email"))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config -global alias.",strlen("nimkat config -global alias."))==0) flag=1;
    else if(strncmp(argv[n],"nimkat config alias.",strlen("nimkat config alias."))==0) flag=1;
    
    if(flag==0) {printf("Invalid command!");return 1;}
    else
    {
        return run_alias(argc,argv);
    }
}
    }
else if(strcmp(argv[1],"add")==0)
{
   if(argc<3) {printf("not a valid command(if you use wild_cards there is not any such file or directory!)");return 1;}
   else if((argc==3)&&(strcmp(argv[2],"-n"))) {general_add(argv[2]);}
   else
   {
        if(strcmp(argv[2],"-f")==0)
        {
            int i=3;
            while(argv[i]!=NULL)
            {
                general_add(argv[i]);
                i++;
            }
        }
        else if(strcmp("-n",argv[2])==0)
        {
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            DIR* dir=opendir(cwd);
            struct dirent* entry;
            while((entry=readdir(dir))!=NULL)
            {
                char final_address[1024];
                strcpy(final_address,cwd);
                strcat(final_address,"\\");
                strcat(final_address,entry->d_name);
                if((strcmp(entry->d_name,".."))&&(strcmp(entry->d_name,"."))&&(entry->d_type==DT_DIR))
                {
                    int a=staged_folder(final_address,1);
                    if(a==1) printf("%s :is sataged\n",final_address);
                    else printf("%s :is not staged\n",final_address);
                }
                else if(strcmp(entry->d_name,".")&&(strcmp(entry->d_name,"..")))
                {
                    int a=is_staged(final_address,1);
                    if(a==1) printf("%s :is sataged\n",final_address);
                    else printf("%s :is not staged\n",final_address);
                }
            }
        }
        else
        {
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            int i=2;
            while(argv[i]!=NULL)
            {
                char final_address[1024]="";
                strcpy(final_address,cwd);
                strcat(final_address,"\\");
                strcat(final_address,argv[i]);
                general_add(final_address);
                i++;
            }
        }
   }
   if(new_addings>0)
   {
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    FILE* file=fopen("last_stages.txt","w");
        for(int i=1;i<=new_addings;i++)
        fprintf(file,"%s\n",store_numbers[i]);
    fclose(file);
   }
}
else if(strcmp(argv[1],"reset")==0)
{
if(existance(".nimkat")==NULL) {printf("you have not initialized yet!"); return 0;}
if(strcmp(argv[2],"-undo")==0)
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    FILE* file1=fopen("last_stages.txt","r");
    char reset_address[1024];
    while(1)
    {
        fgets(reset_address,1024,file1);
        reset_address[strlen(reset_address)-1]='\0';
        if(feof(file1)) break;
        is_staged(reset_address,0);
    }
    fclose(file1);   
}
else
{
    FILE* file=fopen(argv[2],"r");
    DIR* dir=opendir(argv[2]);
    if((file==NULL)&&(dir==NULL)) {printf("insert a valid address!");return 0;}
    if(file!=NULL)
    {
        is_staged(argv[2],0);
    }
    else if(dir!=NULL)
    {
        staged_folder(argv[2],0);
    }
    fclose(file);
    closedir(dir);
}
}

else if(strcmp(argv[1],"status")==0)
{

}

else
{
    if(existance(".nimkat")==NULL) printf("you have not already initialized!");
    char alias_check[500];
    char real_command[1000]="\0";
    FILE* alias=fopen(strcat(existance(".nimkat"),"\\config\\aliases.txt"),"r");
    while(1)
    {
    fgets(alias_check,500,alias);
    if(feof(alias)) break;
    alias_check[strlen(alias_check)-1]='\0';
    if(strcmp(argv[1],alias_check)==0)
    {
        fgets(real_command,1000,alias);
        real_command[strlen(real_command)-1]='\0';
        break;
    }
    }
    fclose(alias);
    if(strcmp(real_command,"\0")==0) {printf("wrong command!");return 1;}
    else
    {
        FILE* help=fopen("G:\\bp_project\\globals\\temporary.txt","w+");
        fprintf(help,"%s",real_command);
        char *word[50];
        for(int j=0;j<50;j++) word[j]=(char*)malloc(50);
        int i=0;
        rewind(help);
        while(1)
        {
            fscanf(help,"%s",word[i]);
            i++;
            if(feof(help)) break;
        }
        fclose(help);
        remove("G:\\bp_project\\globals\\temporary.txt");
        for(int i=0;i<4;i++) printf("%s ",word[i]);
        return main(i,word);
    }
}
}