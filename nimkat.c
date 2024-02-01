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
char output[1024];
char tokenized[20][1024];
char* rel_to_abs(char address[])
{
   if(strstr(address,"\\")!=NULL)  return address ;
    char* token;
    getcwd(output,sizeof(output));
    token=strtok(address,"/");
    while(token!=NULL)
    {
       strcat(output,"\\");
        strcat(output,token);
        token=strtok(NULL,"/");
    }
    return output;
}

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

char* folfile_name(char address[],short int m)
{
    char* token;
    token=strtok(address,"\\");
    int i=0;
    while(token!=NULL)
    {
        strcpy(tokenized[i],token);
        token=strtok(NULL,"\\");
        i++;
    }
    if(m==0) return tokenized[i-1];
    else if (m==1) return tokenized[i-2];
}

int  commit_folder_maker(char commit_message[])
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    int count=0;
    char address[1024];
    char written_time[100];
    char mode_checker[4];
    FILE* stat=fopen("status.txt","r");
    while(1)
    {
        fgets(address,1024,stat);
        address[strlen(address)-1]='\0';
        if(feof(stat)) break;
        fgets(written_time,100,stat);
        written_time[strlen(written_time)-1]='\0';
        fgets(mode_checker,4,stat);
        if((strcmp(mode_checker,"sd\n")==0)||((mode_checker[0]=='s')&&(strcmp(written_time,last_modified_check(address))==0)))
        {
            count++;
        }
    }
    fclose(stat);
    if(count==0) {printf("no stage files exist!\n");return 0;}
    if(count>0)
    {
    chdir("..");
    FILE* file=fopen("last_com.txt","r");
    if(file==NULL) printf("unable to find the current commit number!");
    else
    {
        chdir("config");
        int a;
        fscanf(file,"%d",&a);
        fclose(file);
        a+=1;
        FILE* f=fopen("..\\last_com.txt","w");
        fprintf(f,"%d",a);
        fclose(f);
        FILE* r=fopen("..\\cur_com.txt","w");
        fprintf(r,"%d",a);
        fclose(r);
        char k[10];
        sprintf(k,"%d",a);
        mkdir(k);
        chdir(k);
        FILE* id_num=fopen("id_num.txt","w");
            fprintf(id_num,"%d",a);
            fclose(id_num);
            FILE* user=fopen("..\\name.txt","r");
            char name[51];
            fgets(name,sizeof(name),user);
            fclose(user);
            FILE* author=fopen("author.txt","w");
            fprintf(author,"%s",name);
            fclose(author);
            FILE* message=fopen("message.txt","w");
            fprintf(message,"%s",commit_message);
            fclose(message);
            FILE* commit_time=fopen("time.txt","w");
            time_t t = time(NULL);
            fprintf(commit_time,"%s",ctime(&t));
            fclose(commit_time);
            FILE* branch=fopen("branch.txt","w");
            FILE* current=fopen("../../cur_branch.txt","r");
            char br_name[50];
            fgets(br_name,sizeof(br_name),current);
            fprintf(branch,"%s",br_name);
            fclose(current);
            fclose(branch);
            char source[1024];
            sprintf(source,"%s\\%s\\status.txt",existance(".nimkat"),branch_name);
            FILE* status=fopen(source,"r");
            char dest[1024];
            sprintf(dest,"%s\\config\\%s",existance(".nimkat"),k);
            FILE* ss=fopen("address_of_ss.txt","w");
            char mod[4];
            char adr[1024];
            char written_time[50];
            while(1)
            {
                fgets(adr,sizeof(adr),status);
                adr[strlen(adr)-1]='\0';
                if(feof(status)) break;
                fgets(written_time,sizeof(written_time),status);
                written_time[strlen(written_time)-1]='\0';
                fgets(mod,sizeof(mod),status);
                if((strcmp(written_time,last_modified_check(adr))==0)&&(strcmp(mod,"ss\n")==0))
                {
                    fprintf(ss,"%s\n",adr);
                }
            }
            fclose(ss);
            fclose(status);
            mkdir("ss_files");
            FILE* sss=fopen("address_of_ss.txt","r");
            char addr[1024];
            char com1[2050];
            while(1)
            {
                strcpy(com1,"\0");
                fgets(addr,sizeof(addr),sss);
                addr[strlen(addr)-1]='\0';
                strcpy(com1,"copy ");strcat(com1,addr);strcat(com1," ");strcat(com1,dest);strcat(com1,"\\ss_files\\");strcat(com1,folfile_name(addr,0));
                if(feof(sss)) break;
                system(com1);
            }
            fclose(sss);
            char check[1024];
            char mtime[100];
            char mode[4];
            FILE* change_stat=fopen(source,"r");
            while(1)
            {
                fgets(check,sizeof(check),change_stat);
                if(feof(change_stat)) break;
                fgets(mtime,sizeof(mtime),change_stat);
                int c=ftell(change_stat);
                fgets(mode,sizeof(mode),change_stat);
                if((strcmp(mode,"sd\n")==0)||(strcmp(mode,"ss\n")==0))
                {
                    FILE* f=fopen(source,"r+");
                    fseek(f,c,SEEK_SET);
                    if(strcmp(mode,"sd\n")==0) fprintf(f,"dd\n");
                    else fprintf(f,"uu\n");
                    fclose(f);
                }               
            }
            fclose(change_stat);
            strcat(dest,"\\status.txt");
            char command[2048];
            sprintf(command,"copy %s %s",source,dest);
            system(command);
            FILE* c_num=fopen("commited_num.txt","w");
            fprintf(c_num,"%d",count);
            fclose(c_num);
        
    }
    }
    chdir(cwd);
    return 1;
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

void branch_folder_maker(char br_name[])
{
    char where[1024];
    getcwd(where,sizeof(where));
    chdir(existance(".nimkat"));
    mkdir(br_name);
    chdir(br_name);
    FILE* stat=fopen("status.txt","w");
    fclose(stat);
    FILE* l_stage=fopen("last_stages.txt","w");
    fclose(l_stage);
    FILE* sf=fopen("head_id.txt","w");
    fclose(sf);
    chdir(where);
}

int run_init(int argc,char* argv[])
{
    if(existance(".nimkat")!=NULL) {printf("already initialized!");}
    else
    {
     mkdir(".nimkat");
     mkdir(".nimkat\\config");
     FILE* f=fopen(".nimkat\\cur_branch.txt","w");
     fprintf(f,"%s","master");
     fclose(f);
     branch_folder_maker("master");
     FILE* file=fopen("G:\\bp_project\\globals\\projects.txt","a");
     fprintf(file,"%s\n",existance(".nimkat"));
     fclose(file);
     FILE* t=fopen(".nimkat\\config\\com_shortcs.txt","w");
     fclose(t);
     FILE* c=fopen(".nimkat\\last_com.txt","w");
     fprintf(c,"%d",0);
     fclose(c);
     FILE* d=fopen(".nimkat\\cur_com.txt","w");
     fprintf(d,"%d",0);
     fclose(d);
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
    char time[100];
    char cwd[1024];
    char address_checker[1024];
    getcwd(cwd,sizeof(cwd));
    strcpy(time,last_modified_check(address));
    chdir(existance(".nimkat"));
    chdir(branch_name);
    FILE* add=fopen("status.txt","r+");
    FILE* exist=fopen(address,"r");
    while(1)
    {
        fgets(address_checker,1024,add);
        address_checker[strlen(address_checker)-1]='\0';
        if(feof(add)) break;
    
         if(strcmp(address,address_checker)==0)
        { 
            flag=1;
            char written_time[1024];
            char mode[4];
            int a=ftell(add);
            fgets(written_time,1024,add);
            written_time[strlen(written_time)-1]='\0';
            fgets(mode,4,add);

            if((strcmp(written_time,time)==0)&&(mode[0]=='s'))
            {
            printf("the file is already added!");chdir(cwd);return 1;
            }
            
            else if(strcmp(mode,"dd\n"))
            {
                FILE* addd=fopen("status.txt","r+");
                fseek(addd,a,SEEK_SET);
                fprintf(addd,"%s\n",time);
                if(exist==NULL) fprintf(addd,"sd\n");
                else fprintf(addd,"ss\n");
                fclose(addd);
                new_addings++;
                strcpy(store_numbers[new_addings],address);  
            }
            if(strcmp(mode,"dd\n")==0) printf("the file deletation is commited in previous commits!");
    }
    }
    fclose(add);
    if(flag==0)
    {
        if(exist==NULL) printf("The file faild to find.\n");
        else
        {
            FILE* new_add=fopen("status.txt","a");
            fprintf(new_add,"%s\n",address);
            fprintf(new_add,"%s\n",time);
            fprintf(new_add,"ss\n");
            fclose(new_add);
            new_addings++;
            strcpy(store_numbers[new_addings],address);
        }
    }
    fclose(exist);
    chdir(cwd); 
}

int  general_add(char address[])
{
    if(existance(".nimkat")==NULL) {printf("you have not initialized yet");return 1;}
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    short int flag=0;
    chdir(existance(".nimkat"));
    chdir(branch_name);
    char address_check[1024];
    FILE* status=fopen("status.txt","r");
    while(1)
    {
        fgets(address_check,1024,status);
        address_check[strlen(address_check)-1]='\0';
        if(feof(status)) break;
        if(strcmp(address,address_check)==0) {flag=1;break;}
    }

    if((fopen(address,"r")==NULL)&&(chdir(address)!=0)&&(flag==0)) {printf("the address faild to find\n");return 1;}
    if(chdir(address)!=0)
    {
        return run_add(address);
    }
    else
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
    FILE* exist=fopen(address,"r");
    if(status==NULL) {printf("faild to open status.txt,please contact us!");return -3;}
        while(1)
        {
            fgets(address_checker,1024,status);
            address_checker[strlen(address_checker)-1]='\0';
            if(feof(status)) break;
            if(strcmp(address,address_checker)==0)
            {
                flag=1;
                char mode[4];
                char written_time[200];
                fgets(written_time,200,status);
                written_time[strlen(written_time)-1]='\0';
                int c=ftell(status);
                if(m==1)
                {
                    fgets(mode,4,status);
                    if(strcmp(mode,"dd\n")==0) return -2;
                    if((strcmp(time,written_time)==0)&&(exist!=NULL)&&(strcmp(mode,"ss\n")==0)) return 1;
                    else if((strcmp(time,written_time)==0)&&(exist==NULL)&&(strcmp(mode,"sd\n")==0)) return 2;
                    else if(exist!=NULL) return 0;
                    else if(exist==NULL) return -1;
                }
                if(m==0)
                {
                    if(strcmp(mode,"dd\n"))
                    {
                        fopen("status.txt","r+");
                        fseek(status,c,SEEK_SET);
                        fprintf(status,"u");
                        fclose(status);
                    return 2;
                    }
                }
            }
        }
    fclose(status);
    if(flag==0) 
    {
        if(m==1)
        {
            if(exist!=NULL) return -3;
            return -4;
        } 

        else if(m==0){printf("the file was not staged before!\n");return -4;}
     }
     fclose(exist);
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
if(existance(".nimkat")!=NULL) {char w_dir[1024];getcwd(w_dir,sizeof(w_dir));chdir(existance(".nimkat"));FILE* file=fopen("cur_branch.txt","r");fgets(branch_name,sizeof(branch_name),file);fclose(file);chdir(w_dir);}
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
   else if((argc==3)&&(strcmp(argv[2],"-n"))) {general_add(rel_to_abs(argv[2]));}
   else
   {
        if(strcmp(argv[2],"-f")==0)
        {
            int i=3;
            while(argv[i]!=NULL)
            {
                general_add(rel_to_abs(argv[i]));
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
    char to_reset[1024];
    strcpy(to_reset,rel_to_abs(argv[2]));
    int flag=0;
    chdir(existance(".nimkat"));
    chdir(branch_name);
    FILE* status=fopen("status.txt","r");
    char ad_check[1024];
    while(1)
    {
        fgets(ad_check,1024,status);
        ad_check[strlen(ad_check)-1]='\0';
        if(feof(status)) break;
        if(strcmp(ad_check,to_reset)==0) {flag=1;break;}
    }
    FILE* file=fopen(to_reset,"r");
    DIR* dir=opendir(to_reset);
    if((file==NULL)&&(dir==NULL)&&(flag==0)) {printf("insert a valid address!");return 0;}
    else if(dir==NULL)
    {
        is_staged(to_reset,0);
    }
    else if(dir!=NULL)
    {
        staged_folder(to_reset,0);
    }
    fclose(file);
    closedir(dir);
}
}

else if((strcmp(argv[1],"commit")==0)&&(strcmp(argv[2],"-m")==0))
{
    if(argc>4) {printf("please enter a valid command(don't forget double qoutations for messages including with_space)");return 0;}
    else if(argc==3) {printf("please insert a message for your commit");return 0;}
    else if(strlen(argv[3])>72) {printf("the message is too long(should be atmost 72 characters)");return 0;}
    else
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        int k=commit_folder_maker(argv[3]);
        if(k!=0)
        {
            char c_time[50];
            char id_num[4];
            chdir(existance(".nimkat"));
            FILE* l_commit=fopen("last_com.txt","r");
            fscanf(l_commit,"%s",id_num);
            fclose(l_commit);
            chdir("config");
            chdir(id_num);
            FILE* time=fopen("time.txt","r");
            fgets(c_time,sizeof(c_time),time);
            fclose(time);
            printf("commited successfully!\ncommit id: %s\ntime: %s message: %s",id_num,c_time,argv[3]);
            chdir(cwd);
        }

    }
}

else if(strcmp(argv[1],"set")==0)
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    FILE* shortcut=fopen("config/com_shortcs.txt","a");
    fprintf(shortcut,"%s\n",argv[5]);
    fprintf(shortcut,"%s\n",argv[3]);
    fprintf(shortcut,"------------\n");
    fclose(shortcut);
    chdir(cwd);
}

else if((strcmp(argv[1],"commit")==0)&&(strcmp(argv[2],"-s")==0))
{
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    char name_checker[50];
    char real_message[100];
    int flag=0;
    FILE* shortcut=fopen("config/com_shortcs.txt","r");
    while(1)
    {
        fgets(name_checker,sizeof(name_checker),shortcut);
        name_checker[strlen(name_checker)-1]='\0';
        if(feof(shortcut)) break;
        if(strcmp(name_checker,argv[3])==0)
        {
            flag=1;
            fgets(real_message,sizeof(real_message),shortcut);
            real_message[strlen(real_message)-1]='\0';
            char* ret[4]={"nimkat","commit","-m",real_message};
            return main(4,ret);
        }
    }
    fclose(shortcut);
    if(flag==0) printf("invalid shortcut name.");
    chdir(cwd);  
}

else if(strcmp(argv[1],"replace")==0)
{
    int flag=0;
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    FILE* shortcut=fopen("config/com_shortcs.txt","r");
    FILE* replaced=fopen("config/com_shortcs1.txt","w");
    char name[50];
    char real_message[100];
    char sep[40];
    while(1)
    {
        fgets(name,sizeof(name),shortcut);
        name[strlen(name)-1]='\0';
        if(feof(shortcut)) break;
        fgets(real_message,sizeof(real_message),shortcut);
        fgets(sep,sizeof(sep),shortcut);
        if(strcmp(name,argv[5]))
        {
            fprintf(replaced,"%s\n",name);
            fprintf(replaced,"%s",real_message);
            fprintf(replaced,"%s",sep);
        }
        else
        {
            flag=1;
            fprintf(replaced,"%s\n",name);
            fprintf(replaced,"%s\n",argv[3]);
            fprintf(replaced,"%s",sep);
        }
    }
    fclose(shortcut);
    fclose(replaced);
    if(flag==0) {printf("the shortcut does not exist!");}
    else
    {
        char command[2000];
        sprintf(command,"copy %s\\config\\com_shortcs1.txt %s\\config\\com_shortcs.txt",existance(".nimkat"),existance(".nimkat"));
        system(command);
    }
    remove("config/com_shortcs1.txt");
    chdir(cwd);
}

else if(strcmp(argv[1],"remove")==0)
{
    int flag=0;
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));
    chdir(existance(".nimkat"));
    FILE* shortcut=fopen("config/com_shortcs.txt","r");
    FILE* removed=fopen("config/com_shortcs1.txt","w");
    char name[50];
    char real_message[100];
    char sep[40];
    while(1)
    {
        fgets(name,sizeof(name),shortcut);
        name[strlen(name)-1]='\0';
        if(feof(shortcut)) break;
        fgets(real_message,sizeof(real_message),shortcut);
        fgets(sep,sizeof(sep),shortcut);
        if(strcmp(name,argv[3]))
        {
            fprintf(removed,"%s\n",name);
            fprintf(removed,"%s",real_message);
            fprintf(removed,"%s",sep);
        }
        else
        {
            flag=1;
        }
    }
    fclose(shortcut);
    fclose(removed);
    if(flag==0) {printf("the shortcut does not exist!");}
    else
    {
        char command[2000];
        sprintf(command,"copy %s\\config\\com_shortcs1.txt %s\\config\\com_shortcs.txt",existance(".nimkat"),existance(".nimkat"));
        system(command);
    }
    remove("config/com_shortcs1.txt");
    chdir(cwd);
    
}

else if(strcmp(argv[1],"log")==0)
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