#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_num(char *str);
int get_set(int bsize,int way,int csize);
void show_mr(int bsize, int way, int csize,int what_cache,char *link, int option);
int LRUevict(int** cache,int way,int tag);
void LRUupdate(int** set,int block,int way);

int main() {
    int num=1,select;
    char name[20]="";
    printf("Cache simulator \n2014313795 김성섭\n");
    printf("Computer architecture assignment2\n");
    printf("Select file\n1. trace1.din\n2. trace1-short.din\n3. trace2.din\n4. trace2-short.din\n");
    while (num) {
        scanf("%d",&select);
        switch (select) {
            case 1:
                strcpy(name,"trace1.din");
                num = 0;
                break;
            case 2:
                strcpy(name,"trace1-short.din");
                num = 0;
                break;
            case 3:
                strcpy(name,"trace2.din");
                num = 0;
                break;
            case 4:
                strcpy(name,"trace2-short.din");
                num =0;
                break;
            default:
                printf("please try again.\n");
                break;
        }
    }
    printf("%d. %s is selected.\n",select,name);
    int what_cache,bsize, way, csize;
    for(what_cache=0; what_cache<2 ; what_cache++) {
        for(bsize= 16 ; bsize<65 ; bsize*=4) {
            if(what_cache ==0) printf("\nL1 I-cache table\n");
            else printf("\nL1 D-cache table\n");
            printf("Cache Miss Ratio (block size = %dB)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize,bsize);
            for(way = 1; way < 9 ; way*=2) {
                if(way==1) printf("Direct  |");
                else if(way==2) printf("2-way   |");
                else if(way==4) printf("4-way   |");
                else printf("8-way   |");
                for(csize = 1024; csize <=16385 ; csize*=2) {
                    show_mr(bsize,way,csize,what_cache,name,0);
                }
                printf("\n");
            }
        }
    }

    for(bsize= 16 ; bsize<65 ; bsize*=4) {
        printf("\nL1 D-cache table\n");
        printf("Number of Memory Block Writes (Inclusive)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize);
        for(way = 1; way < 9 ; way*=2) {
            if(way==1) printf("Direct  |");
            else if(way==2) printf("2-way   |");
            else if(way==4) printf("4-way   |");
            else printf("8-way   |");
            for(csize = 1024; csize <16385 ; csize*=2) {
                show_mr(bsize,way,csize,1,name,1);
            }
            printf("\n");
        }
    }
    for(bsize= 16 ; bsize<65 ; bsize*=4) {
        printf("\nL1 D-cache table\n");
        printf("Number of Memory Block Writes (Exclusive)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize);
        for(way = 1; way < 9 ; way*=2) {
            if(way==1) printf("Direct  |");
            else if(way==2) printf("2-way   |");
            else if(way==4) printf("4-way   |");
            else printf("8-way   |");
            for(csize = 1024; csize <16835 ; csize*=2) {
                show_mr(bsize,way,csize,1,name,2);
            }
            printf("\n");
        }
    }
    return 0;
}
//main 종료
long get_num(char *str) {
    char *ptr;
    long result;
    ptr = strtok(str, " ");
    ptr = strtok(NULL,"\n");
    result = strtol(ptr, NULL, 16);
    return result;
}

int get_set(int bsize, int way, int csize) {       
    return csize/(bsize*way);
}
void show_mr(int bsize, int way, int csize,int what_cache,char *link,int option) {
    FILE *trace;
    char str[20];
    long addr;
    int v,index,block,tag,i_cnt=0,i_num_hit=0,d_cnt=0,d_num_hit=0,num_wb=0,num_wb2=0,L2_cnt=0,L2_num_hit=0;
    int num_set= get_set(bsize,way,csize),L2_num_set = get_set(bsize,8,16384);
    int ***icache = (int***) malloc(sizeof(int**)*num_set);
    int ***dcache = (int***) malloc(sizeof(int**)*num_set);
    int ***L2cache_in= (int***) malloc(sizeof(int**)*L2_num_set);
    int ***L2cache_ex= (int***) malloc(sizeof(int**)*L2_num_set);
    for ( index = 0 ; index < num_set ; index++) {                     
        icache[index] = (int**)malloc(sizeof(int*)*way);            //index는 (set의 순서)를 의미한다.
        dcache[index] = (int**)malloc(sizeof(int*)*way);
        for( block = 0; block <way ; block++) {
            icache[index][block] = (int*)calloc(sizeof(int),3);         //block은 block의 순서를 따지며
            dcache[index][block] = (int*)calloc(sizeof(int),3);         //icache[i][j][0]부터 ...[2]까지 각각 valid bit,tag 정보,dirty bit를 의미
        }                                                       //valid bit와 dirty bit의 0으로 초기화가 필요하므로 malloc함수 대신 calloc함수 사용
    }
    for ( index = 0 ; index < L2_num_set ; index++ ) {
        L2cache_in[index] = (int**)malloc(sizeof(int*)*8); 
        L2cache_ex[index] = (int**)malloc(sizeof(int*)*8); 
            for(block=0; block<8; block++){
                L2cache_in[index][block] = (int*)calloc(sizeof(int),3);
                L2cache_ex[index][block] = (int*)calloc(sizeof(int),3);
            }
    }
    float i_rate, d_rate;
    char link2[30] = "trace/";
    strcat(link2,link);
    trace = fopen(link2,"r");
    while (fgets(str,20,trace)) {
        addr = get_num(str);                                   //예전에 잘못된 것 : word_addr(=n/4)/way **way는 영향을 끼치지 않는다 num_set을 구할때빼고는
        index = (addr/bsize)%num_set;                        // n/bsize는 tag부분 + index부분 
        tag = (addr/bsize)/num_set;                         // 그부분을 나눠줄수있는것이 num_set으로 나누기
        if(str[0]=='2') {                                       //instruction read
            for(block=0; block<way; block++) {              //index가 맞는 줄의 모든 block을 뒤진다.
                if(icache[index][block][0]==1 && icache[index][block][1]== tag) { 
                    i_num_hit++;                             //valid bit가 1이고 tag도 매칭되어 hit한 상황
                    LRUupdate(icache[index],block,way);
                    break;                                  //hit 했으므로 더이상 볼것없다 그냥읽자.
                }
                else if (icache[index][block][0]==0) {          //miss인 상황1.  빈 공간이있다
                    icache[index][block][0] = 1;                //가장 최신블록에 블록을 추가
                    icache[index][block][1] = tag;
                    icache[index][block][2] = 0;
                    break;
                }
                else if (block==way-1 && icache[index][block][0] == 1) { //miss이고 전부 꽉 차있어서 evict해야하는 상황
                    LRUevict(icache[index],way,tag);
                }
            }
            i_cnt++;
        }
        else if(str[0]=='0'|| str[0]=='1') {                              //data read
            for(block=0; block<way ;block++) {              //index가 맞는 줄의 모든 block을 뒤진다.
                if(dcache[index][block][0]==1 && dcache[index][block][1]== tag) { 
                    d_num_hit++;                            //valid bit가 1이고 tag도 매칭되어 hit한 상황
                    if(str[0]=='1')dcache[index][block][2]=1;// dirtybit만 1로만들고 지연쓰기 예약하고 캐시에쓰기
                    LRUupdate(dcache[index],block,way);
                    break;                                  //hit 했으므로 더이상 볼것없다 이 명령에 대한건 이제종료
                }
                else if (dcache[index][block][0]==0) {                  //tag matching도 아니고(miss) 빈 공간이있다
                    dcache[index][block][0] = 1;
                    dcache[index][block][1] = tag;
                    dcache[index][block][2] = 0;
                    break;
                }
                else if (block==way-1 && dcache[index][block][0] == 1) { //miss이고 전부 꽉 차있어서 evict해야하는 상황
                    if(str[0]=='1') {
                        num_wb+=LRUevict(dcache[index],way,tag);
                        num_wb2++;
                    }
                    else LRUevict(dcache[index],way,tag);
                }
            }
            d_cnt++;
        }
    }       //while end자
    i_rate = (float)i_num_hit/i_cnt;
    d_rate = (float)d_num_hit/d_cnt;
    if(what_cache==0) printf("%5.2f%% |",(1-i_rate)*100);
    else {
        if(option==1) printf("%7d|",num_wb);
        else if(option==2) printf("%7d|",num_wb2);
        else printf("%5.2f%% |",(1-d_rate)*100);
    }

    //메모리 해제시간
    for ( index = 0 ; index < num_set ; index++) {
        for(block=0; block<way; block++) {
            free(icache[index][block]);
            free(dcache[index][block]);
        }
        free(icache[index]);
        free(dcache[index]);
    }
    for ( index = 0 ; index < L2_num_set ; index++ ) {
        for(block=0; block<8; block++) {
            free(L2cache_in[index][block]);
            free(L2cache_ex[index][block]);
        }
        free(L2cache_in[index]);
        free(L2cache_ex[index]);
    }
    free(L2cache_in);
    free(L2cache_ex);
    free(icache);
    free(dcache);
    fclose(trace);
}

int LRUevict(int** set,int way,int tag) {        //다찼다 내보내
    int result = 0,block;                                //Least recently used 전략을 사용
    if(set[0][2] ==1) {   //dirty bit on, 지연쓰기
        set[0][2]=0;      //동시에 dirtybit초기화
        result++;           //memory block wirte를 counting해주기 위해 이것을 사용합니다.
    }
    //0에서 way-2까지 설정
    for(block=0; block<way-1; block++) {              //순차적으로 앞으로 땡기기
        set[block][0]=set[block+1][0];
        set[block][1]=set[block+1][1];
        set[block][2]=set[block+1][2];
    }
    //way-1 가장 최신블록의 정보 설정
    set[way-1][0] = 1;      //마지막 블록은 최신 블록,즉  lru에서 가장 먼 블록..
    set[way-1][1] = tag;    //최신블록갱신
    set[way-1][2] = 0;
    return result;
}
void LRUupdate(int** set,int block,int way) {
    int block_tmp[3];
    if(block!=way-1) {
        block_tmp[0]=set[block][0];
        block_tmp[1]=set[block][1];
        block_tmp[2]=set[block][2];
        for(block=block;block<way-1;block++) {
            if (set[block+1][0]==1) {                          //앞에 자료가있다, 그럼 땡겨온다
                set[block][0]=set[block+1][0];
                set[block][1]=set[block+1][1];
                set[block][2]=set[block+1][2];
            }
            else if (set[block+1][0]==0) {          //앞에 빈 공간이있다,여기가 최신공간이다.
                set[block][0]=block_tmp[0];
                set[block][1]=block_tmp[1];
                set[block][2]=block_tmp[2];
                break;
            }
        }
    }
}
