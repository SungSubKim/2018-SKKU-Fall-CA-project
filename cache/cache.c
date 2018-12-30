#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_num(char *str);                        //din파일의 string에서 16진수 숫자를 추출하여 반환하는 함수입니다.
int get_set(int bsize,int way,int csize);       //blocksize와 way의 크기, cache의 전체사이즈를 고려하여 set의 수를 반환합니다.
void show_mr(int bsize, int way, int csize,int what_cache,char *link, int option);  //cache computing을 simulation하여 해당되는 cache의 missrate을 보여주고, option에 따라 missrate이 아닌 지연쓰기 횟수를 보여줍니다.
int LRUevict(int** set,int way,int tag,int option);         //option:0 -> LRU 정책에 따라 가장 오래된 blcok을 제거하고, 새로운 최신block을 추가해줍니다.
                                                        //option:1 -> 해당되는 set에서 해당되는 tag를 가진 blcok만을 제거합니다.
void LRUupdate(int** set,int block,int way);            //해당되는 블록을 LRU정책에 따라 가장 최신 블록의 위치에 위치시킵니다.
int addtoL2in(int*** L2cache, int blockaddr,int* L2rate,int num_setL2,int*** icache, int*** dcache, int num_setL1,int wayL1,int isDcache,int option,int write);
                                                        //L2 inclusive policy에 따라 block을 L2 inclusive cache에 추가해줍니다.
                                                        //L1 miss시에만 발동됩니다.
int accesstoL2ex(int*** L2cache, int blockaddr,int* L2rate,int num_setL2,int*** icache, int*** dcache, int num_setL1,int wayL1,int isDcache,int option);
                                                        //L2 exclusive policy에 따라 block을 L2 cache에 추가해줍니다.
                                                        //해당 함수 또한 L1 miss시에만 발동됩니다.

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
    //입력받기 완료
    int what_cache,bsize, way, csize;
    for(what_cache=0; what_cache<4 ; what_cache++) {//what_cache ==0 -> instruction cache, 1-> data cache, 2->L1 inclusvie, 3->L2 exclusvie
        for(bsize= 16 ; bsize<65 ; bsize*=4) {      //blcok size 16과 64인 case에 대하여 table 출력
            if(what_cache==0 || what_cache ==1) {
                if(what_cache ==0) printf("\nL1 I-cache table\n");
                else printf("\nL1 D-cache table\n");
                printf("Cache Miss Ratio (block size = %dB)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize,bsize);
            }
            else if(what_cache==2) {                     //
                printf("\nL2 Miss Ratio (block size = %d B) (Inclusive)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize,bsize);
            }
            else if(what_cache==3) {
                printf("\nL2 Miss Ratio (block size = %d B) (Exclusive)\nLRU/%d  | 1024  | 2048  | 4096  | 8192  | 16384 |\n",bsize,bsize);
            }                           //각각 cache의 경우에 따른 printf string출력
            for(way = 1; way < 9 ; way*=2) {    //direct-size -> way =1, way2,way4,way8,...
                if(way==1) printf("Direct  |");
                else if(way==2) printf("2-way   |");
                else if(way==4) printf("4-way   |");
                else printf("8-way   |");
                for(csize = 1024; csize <=16385 ; csize*=2) {       //cache size에 따라(1024,2048,..) miss rate출력
                    show_mr(bsize,way,csize,what_cache,name,0);
                }
                printf("\n");
            }
        }
    }

    for(bsize= 16 ; bsize<65 ; bsize*=4) {  // datacache block write(inclusive)에 대한 출력
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
    for(bsize= 16 ; bsize<65 ; bsize*=4) {  // datacache block write(exclusive)에 대한 출력
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

long get_num(char *str) {       // 한줄의 스트링에 대하여 주소부분 16진수 숫자만 추출하여 반환하는 함수
    char *ptr;
    long result;
    ptr = strtok(str, " ");
    ptr = strtok(NULL,"\n");
    result = strtol(ptr, NULL, 16);
    return result;
}

int get_set(int bsize, int way, int csize) {       //조건내에서의 cache내에 있는 set의 수를 반환한다.
    return csize/(bsize*way);
}

void show_mr(int bsize, int way, int csize,int what_cache,char *link,int option) {
    FILE *trace;
    char str[20];           //파일위치를 다룰때 사용
    long addr,blockaddr;    //addr에 string내의 16진수 숫자가 담기며, 이 byteadress를 blocksize로 나눈것이 blockaddr이다.
    float L2_rate_in2, L2_rate_ex2;
    int tmp,index,block,tag,i_cnt=0,i_num_hit=0,d_cnt=0,d_num_hit=0,num_wbin=0,num_wbex=0,victimtag,vblockaddr;
    int num_set= get_set(bsize,way,csize),num_setL2 = get_set(bsize,8,16384);   //각각의 상황에 따른 L1 cache와 L2cache의 세트의 수를 계산
    int ***icache = (int***) malloc(sizeof(int**)*num_set);
    int ***dcache = (int***) malloc(sizeof(int**)*num_set);
    int ***L2cache_in= (int***) malloc(sizeof(int**)*num_setL2);
    int ***L2cache_ex= (int***) malloc(sizeof(int**)*num_setL2);
    for ( index = 0 ; index < num_set ; index++) {                     
        icache[index] = (int**)malloc(sizeof(int*)*way);            //index는 (set의 순서)를 의미한다.
        dcache[index] = (int**)malloc(sizeof(int*)*way);
        for( block = 0; block <way ; block++) {
            icache[index][block] = (int*)calloc(sizeof(int),3);         //block은 block의 순서를 따지며
            dcache[index][block] = (int*)calloc(sizeof(int),3);         //icache[i][j][0]부터 ...[2]까지 각각 valid bit,tag 정보,dirty bit를 의미
        }                                                       //valid bit와 dirty bit의 0으로 초기화가 필요하므로 malloc함수 대신 calloc함수 사용
    }
    for ( index = 0 ; index < num_setL2 ; index++ ) {
        L2cache_in[index] = (int**)malloc(sizeof(int*)*8); 
        L2cache_ex[index] = (int**)malloc(sizeof(int*)*8); 
            for(block=0; block<8; block++){
                L2cache_in[index][block] = (int*)calloc(sizeof(int),3);
                L2cache_ex[index][block] = (int*)calloc(sizeof(int),3);
            }
    }
    int *L2_rate_in = (int*)calloc(sizeof(int), 3);
    int *L2_rate_ex = (int*)calloc(sizeof(int),3);
    float i_rate, d_rate;
    char link2[30] = "trace/";
    strcat(link2,link);
    trace = fopen(link2,"r");
    while (fgets(str,20,trace)) {
        addr = get_num(str);                                   //예전에 잘못된 것 : word_addr(=n/4)/way **way는 영향을 끼치지 않는다 num_set을 구할때빼고는
        blockaddr = addr/bsize;
        index = blockaddr%num_set;                        // n/bsize는 tag부분 + index부분 
        tag = blockaddr/num_set;                         // 그부분을 나눠줄수있는것이 num_set으로 나누기
        if(str[0]=='2') {                                       //instruction read
            for(block=0; block<way; block++) {              //index가 맞는 줄의 모든 block을 뒤진다.
                if(icache[index][block][0]==1 && icache[index][block][1]== tag) { 
                    i_num_hit++;                             //valid bit가 1이고 tag도 매칭되어 hit한 상황
                    LRUupdate(icache[index],block,way);     //접근하였으므로 LRU상의 가장 최신으로 업데이트해줌
                    break;                                  //hit 했으므로 더이상 볼것없다 그냥 읽고 빠져나오자.
                }
                else if (icache[index][block][0]==0) {          //앞의 valid bit가 1인공간을 뒤졌으나 tag에 맞는 block이 존재하지 않았다
                                                                //->cache miss이고, 또한 빈 공간이 존재한다.
                    icache[index][block][0] = 1;                //가장 최신블록에 블록을 추가
                    icache[index][block][1] = tag;
                    icache[index][block][2] = 0;
                    //L1 miss이므로 이에 따라 L2 cache에 접근한다.
                    addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,0,0,0);
                    accesstoL2ex(L2cache_ex,blockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,0,0);
                    break;
                }
                else if (block==way-1 && icache[index][block][0] == 1) { //miss이고 전부 꽉 차있어서 evict해야하는 상황
                    //L1의 block eviction에 앞서 victimblock에 대한 처리를 해준다. option ==1
                    //blcok eviction에 따른 L2캐시 접근이므로 missrate엔 반영이 되지 않게 option을 1로 한다.
                    victimtag=icache[index][0][1];
                    vblockaddr=victimtag*num_set + index;
                    addtoL2in(L2cache_in,vblockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,0,1,0);
                    accesstoL2ex(L2cache_ex,vblockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,0,1);
                    // inclusive: Block eviction from L1 cache: move the victim block to L2 cache (do not update the LRU order of L2 cache.)
                    // exclusive: Block eviction from L1 cache: move the victim block to L2 cache, and update the LRU order of L2 cache

                    LRUevict(icache[index],way,tag,0);  //제거하고 requested block추가한다.
                    addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,0,0,0);
                    accesstoL2ex(L2cache_ex,blockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,0,0);
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
                    //L1에 requested block 추가후 L2에 대한 접
                    if(str[0]=='1' )num_wbin += addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,1,0,1);
                    else addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,1,0,0);
                    num_wbex += accesstoL2ex(L2cache_ex,blockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,1,0);
                    break;
                }
                else if (block==way-1 && dcache[index][block][0] == 1) { //miss이고 전부 꽉 차있어서 evict해야하는 상황
                    victimtag=dcache[index][0][1];
                    vblockaddr=victimtag*num_set + index;
                    num_wbin += addtoL2in(L2cache_in,vblockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,1,1,0);
                    num_wbex += accesstoL2ex(L2cache_ex,vblockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,1,1);

                    if(str[0]=='1') {
                        tmp = LRUevict(dcache[index],way,tag,0);
                        num_wbin += tmp;
                        num_wbex += tmp;
                    }
                    else LRUevict(dcache[index],way,tag,0);

                    if(str[0]=='1') num_wbin += addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,1,0,1);
                    else addtoL2in(L2cache_in,blockaddr,L2_rate_in,num_setL2,icache,dcache,num_set,way,1,0,0);
                    num_wbex += accesstoL2ex(L2cache_ex,blockaddr,L2_rate_ex,num_setL2,icache,dcache,num_set,way,1,0);
                    
                }
            }
            d_cnt++;
        }
    }       //while end자
    i_rate = (float)i_num_hit/i_cnt;
    d_rate = (float)d_num_hit/d_cnt;
    L2_rate_in2=(float)L2_rate_in[0]/L2_rate_in[1];
    L2_rate_ex2=(float)L2_rate_ex[0]/L2_rate_ex[1];
    //print시간
    if(what_cache==0) printf("%5.2f%% |",(1-i_rate)*100);
    else if(what_cache==1) {
        if(option==1) printf("%7d|",num_wbin);
        else if(option==2) printf("%7d|",num_wbex);
        else printf("%5.2f%% |",(1-d_rate)*100);
    }
    else if(what_cache==2) printf("%5.2f%% |",(1-L2_rate_in2)*100);
    else printf("%5.2f%% |",(1-L2_rate_ex2)*100);

    //메모리 해제시간
    for ( index = 0 ; index < num_set ; index++) {
        for(block=0; block<way; block++) {
            free(icache[index][block]);
            free(dcache[index][block]);
        }
        free(icache[index]);
        free(dcache[index]);
    }
    for ( index = 0 ; index < num_setL2 ; index++ ) {
        for(block=0; block<8; block++) {
            free(L2cache_in[index][block]);
            free(L2cache_ex[index][block]);
        }
        free(L2cache_in[index]);
        free(L2cache_ex[index]);
    }
    free(L2cache_in);
    free(L2cache_ex);
    free(L2_rate_in);
    free(L2_rate_ex);
    free(icache);
    free(dcache);
    fclose(trace);
}

int LRUevict(int** set,int way,int tag,int option) {        //option :0 ->기존 기능,오래된놈  내보내고 원하는 블록 집어넣기
                                                            //option :1 -> 추가된 기능, 해당되는 내보내고 그외의 작업은 하지않음.LRU랑 상관이 적긴함.
    int result = 0,block;                                //Least recently used 전략을 사용
    if(option== 0) {                //option 0
        if(set[0][2] ==1) {   //dirty bit on, 지연쓰기
            set[0][2]=0;      //동시에 dirtybit초기화
            result++;           //memory block wirte를 counting해주기 위해 이것을 사용합니다.
        }
        //0에서 way-2까지 설정
        for(block=0; block<way-1; block++) {              //순차적으로 앞으로 땡기기
            set[block][0]=set[block+1][0];
            set[block][1]=set[block+1][1];
            set[block][2]=set[block+1][2];
            if(set[block][0] == 0) break;       //block이  way-1로 나가는 경우가 있다.
        }                                       // way-1로 나가는 경우: 전부다 차있다. 그래서 땡긴결과 way-2까지 다차고 way-1만 남은것
                                                // 전부다 꽉차있었기 때문에 valid bit가 0에 걸리는 경우가 없고 break로 빠지지 않고 for문을 전부 완료
                                                // 따라서 block은 way-1의 값이 되고, 그 위치에 최신 블록이 들어간다.
                                                // 그외 way-2,way-3,... , 1, 0으로 나가는 경우 전부가능.
        set[block][0] = 1;      //마지막 블록은 최신 블록,즉  lru에서 가장 먼 블록..
        set[block][1] = tag;    //최신블록갱신
        set[block][2] = 0;
    } 
    else {              //option이 1인 경우, 해당되는 블록만제거하자.
        for(block=0; block<way-1; block++) {
            if(set[block][0]==1 && set[block][1] ==tag) {
                if(set[block][2] ==1) {   //dirty bit on, 지연쓰기
                    set[0][2]=0;      //동시에 dirtybit초기화
                    result++;           //memory block wirte를 counting해주기 위해 이것을 사용합니다.
                }
                for(; block<way-1; block++) {   //hit된 그 위치로부터 전부 앞으로 한칸씩 땡겨버린다.
                    set[block][0]=set[block+1][0];
                    set[block][1]=set[block+1][1];
                    set[block][2]=set[block+1][2];
                    if(set[block][0] == 0) break;
                }
                break;
            }
        }
    }
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
                if(block==way-2) {                              //여기서 생긴 가장 큰 버그: 꼭 빈 공간이 있어야만 LRUupdate가 가능하다?!
                    set[way-1][0]=block_tmp[0];                 //아니다 다 있을때에도 LRU update가 가능하다.
                    set[way-1][1]=block_tmp[1];
                    set[way-1][2]=block_tmp[2];
                }
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
int addtoL2in(int*** L2cache, int blockaddr,int* L2rate,int num_setL2,int*** icache, int*** dcache, int num_setL1,int wayL1,int isDcache,int option,int write) {
        //option:0 -> L1 miss& L2 hit or miss, missrate 계산이 반영
        //option:1 -> L1 eviction에 수반되어 발생한 함수,따라서 missrate 계산x
        //option:1 -> Block eviction from L1 cache: move the vitim block to L2 cache(do not update the LRU order of L2 cache)
        int result =0,wayL2=8,indexL2,tagL2,blockL2,victimtag,vblockaddr,indexL1,tagL1,blockL1,cnt;
        indexL2 = blockaddr%num_setL2;
        tagL2 = blockaddr/num_setL2;
        //requested block을 L1에 추가하는 거는 show_mr에서 이미 이루어진다.
        // 따라서 여기에서는 L2 hit시 LRUupdate, L2 miss시 L2에 requested block추가만 처리해주고,
        // 그외 counting처리를 따로해준다.
        // L1 miss & L2 hit: add the requested block in L1 cache, updated the LRU order of L2 cache blocks.
        // L1 miss & L2 miss: add the requested block in both L1 cache and L2 cache.
        for(blockL2=0; blockL2<wayL2 ;blockL2++) {              //index가 맞는 줄의 모든 block을 뒤진다.
            if(L2cache[indexL2][blockL2][0]==1 && L2cache[indexL2][blockL2][1]== tagL2) { 
                if(option==0){
                    L2rate[0]++;                            //valid bit가 1이고 tag도 매칭되어 hit한 상황 option이 0인 경우, hitrate계산
                    LRUupdate(L2cache[indexL2],blockL2,wayL2);  //또한 blcok eviction에 따른 실행에서는 LRU order를 업데이트를 하지 않는다.
                }
                if(write==1){
                    L2cache[indexL2][blockL2][2]=1;
                }
                break;                                  //hit 했으므로 더이상 볼것없다 이 명령에 대한건 이제종료
            }
            else if (L2cache[indexL2][blockL2][0]==0) {                  //tag matching도 아니고(miss) 빈 공간이있다
                L2cache[indexL2][blockL2][0] = 1;
                L2cache[indexL2][blockL2][1] = tagL2;
                L2cache[indexL2][blockL2][2] = 0;
                break;
            }
            else if (blockL2==wayL2-1 && L2cache[indexL2][blockL2][0] == 1) { //miss이고 전부 꽉 차있어서 evict해야하는 상황
                //When a block is evicted in L2cache, the same block in L1 cache also must be evicted.
                //L2에서 없어질 블록 그러니까 L2의 0번째 블록을 L1에서도 제거하자!
                victimtag=L2cache[indexL2][0][1];
                vblockaddr=victimtag*num_setL2+indexL2;
                indexL1=vblockaddr%num_setL1;
                tagL1=vblockaddr/num_setL1;
                //L1에서의 same block의 blockaddress를  계산해준다.
                for(blockL1=0; blockL1<wayL1; blockL1++) {
                    if(isDcache==0 && icache[indexL1][blockL1][0]== 1 && icache[indexL1][blockL1][0] == tagL1) {    //isdcache==0 ->icache를 수색한다.
                        LRUevict(icache[indexL1],wayL1,tagL1,1);      //해당 block제거 명령.option은 1로
                        break;
                    }
                    else if(isDcache==1 && dcache[indexL1][blockL1][0]== 1 && dcache[indexL1][blockL1][0] == tagL1) {
                        LRUevict(dcache[indexL1],wayL1,tagL1,1);
                        break;
                    }
                }
                //제거완료.(vicitm block이 L1에 존재하는 경우에 한하여)
                result +=LRUevict(L2cache[indexL2],wayL2,tagL2,0);   //victim block을 제거하고 requested block을 L2에 추가해준다.
                    //evict에 수반되는 writeback수를 카운팅해준다..
            }
        }
        if(option==0)L2rate[1]++;
        return result;
}
int accesstoL2ex(int*** L2cache, int blockaddr,int* L2rate,int num_setL2,int*** icache, int*** dcache, int num_setL1,int wayL1,int isDcache,int option) {
        //option: 0 -> 통상의 L1miss에 따른 L2접근 따라서 missrate에 대한 계산은 이때만 이루어짐
        //option: 1 _> L1 eviction에 따른 L1을 L2로 옮기는 작업. missrate 계산x
        int result=0,wayL2=8,indexL2,tagL2,blockL2,victimtag,vblockaddr,indexL1,tagL1,blockL1,cnt;
        indexL2 = blockaddr%num_setL2;
        tagL2 = blockaddr/num_setL2;
        for(blockL2=0; blockL2<wayL2 ;blockL2++) {              //index가 맞는 줄의 모든 block을 뒤진다.
            if(L2cache[indexL2][blockL2][0]==1 && L2cache[indexL2][blockL2][1]== tagL2) { 
                if(option==0) { //L1 miss & L2 hit: add the requested block in L1 cache, and remove the block in L2 cache
                    L2rate[0]++;                            //valid bit가 1이고 tag도 매칭되어 hit한 상황
                    result+=LRUevict(L2cache[indexL2],wayL2,tagL2,1);       //해당되는 block을 remove하고 그에 따른 지연쓰기를 result에 count
                }
                else if(option==1) {        //L1 eviction에 따른 블록추가접근을 하려는데 기존에 이미 데이터가 있는경우,
                    L2cache[indexL2][blockL2][2] = 1;           //dirtybit만 활성화한다.그러고 break로 빠져나가자!
                    LRUupdate(L2cache[indexL2],blockL2,wayL2);  //또한 해당block에 대하여 접근을 하였으므로 LRUupdate를 해준다.
                }
                break;                                  //hit 했으므로 더이상 볼것없다 이 명령에 대한건 이제종료
            }
            // L1 miss & L2 miss: add the requested block only in L1 cache.
            // 이미 show_mr함수에서 L1에 추가는 완료되었기에 여기서 option이 0일때에 할일은 없다
            //오로지 option이 1일때에 대해서만 다룬다.
            else if (option==1 && L2cache[indexL2][blockL2][0]==0) {                  //빈 공간이있다 victimblock 여기에 놓자
                L2cache[indexL2][blockL2][0] = 1;
                L2cache[indexL2][blockL2][1] = tagL2;
                L2cache[indexL2][blockL2][2] = 0;
                break;
            }
            else if (option==1 && blockL2==wayL2-1 && L2cache[indexL2][blockL2][0] == 1) { //추가해야긴 하는데 꽉차있있어서 evict해야하는 상황
                result+=LRUevict(L2cache[indexL2],wayL2,tagL2,0);
            }
        }
        if(option==0)L2rate[1]++;
        return result;
}
