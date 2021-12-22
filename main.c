#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FOPEN_ERROR -1
#define SUCCESS 1
#define TARGET_PER_STEP 10
#define DP_ARRAY_MAX 21010
int dp[DP_ARRAY_MAX][DP_ARRAY_MAX];

int read_data(char filename[], char dat1[], char dat2[]){
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL) return FOPEN_ERROR;

    fscanf(fp, "%s%s", dat1, dat2);

    fclose(fp);
    return SUCCESS;
}

int read_answer(char filename[], int *dat1, int *dat2){
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL) return FOPEN_ERROR;

    fscanf(fp, "%d%d", dat1, dat2);

    fclose(fp);
    return SUCCESS;
}

int not_equals(int a, int b){
    return a != b;
}

int min(int a, int b){
    return a < b ? a : b;
}

/* 
文字列a, bの編集距離を求める
a: char配列
b: char配列
na: aの長さ
nb: bの長さ
*/
int editdis(char a[], int na, char b[], int nb){
    int i, j;

    for(i = 0; i <= na; i++){
        dp[i][0] = i;
    }
    for(i = 0; i <= nb; i++){
        dp[0][i] = i;
    }

    //dp
    for(i = 1; i <= na; i++){
        for(j = 1; j <= nb; j++){
            dp[i][j] = min(dp[i-1][j-1] + not_equals(a[i-1], b[j-1]),
                min(dp[i-1][j]+1, dp[i][j-1]+1));
            
        }
    }
    return dp[na][nb];
}

void printstring(char s[], int n){
    int i;
    for(i = 0; i < n; i++){
        putchar(s[i]);
    }
    puts("");
}

int main(int argc, char *argv[]){
    char x[51000], y[51000]; //observed data
    char s[51000], l[51000]; //correct data

    char e1[51000], e2[51000]; // correct answer
    char a1[51000], a2[51000]; // my answer
// - 正しく観測した場合は，'.'
// - 余計な記録を追加した場合は，'i'
// - 記録しそびれた場合は，'d'
// - 記録間違いをした場合は，'s'
    // printf("%s, %s", argv[1], argv[2]);
    //idata is correct data
    //odata is observed data
    read_data(argv[1], s, l);
    read_data(argv[2], x, y);
    // read_data("edata", e1, e2);

    int score = 0;
    int i, j;
    int flag = 0;
    for(i = 0; i < 50000; i++){
        //printstring(s+i, TARGET_PER_STEP+1);
        //printstring(x+flag, TARGET_PER_STEP+1);

        int correct = editdis(s+i, TARGET_PER_STEP, x+flag, TARGET_PER_STEP);
        int yokei = editdis(s+i, TARGET_PER_STEP, x+flag+1, TARGET_PER_STEP);
        int sobire = editdis(s+i+1, TARGET_PER_STEP, x+flag, TARGET_PER_STEP);
        // int miss = editdis(s+i+1, TARGET_PER_STEP, x+flag+1, TARGET_PER_STEP);

        // 判別
        if(yokei < correct && yokei <= sobire){
            // yokei
            a1[i] = 'i'; flag+=2;
        } else if(sobire < correct && sobire <= yokei){
            // sobire
            a1[i] = 'd';
        } else{
            // correct or miss
            if (s[i] == x[flag]){
                a1[i] = '.'; flag++;
            } else {
                a1[i] = 's'; flag++;
            }            
        }

        //printf("answer>%c, my>%c\n", e1[i], a1[i]);
        //printf("%d\n", e1[i] == a1[i]);
        score += a1[i] != '.';
        // printf("%c", a1[i]);
    }

    //printstring(a1, 50000);
   
    //int d = editdis(x, flag, s, i);
    //printf("%d\n", d);


	int count,countp = 0;

	for(i=0;i<50000;i+=100){
        // count = 100;
		for(j=0;j<100;j++){
			if(a1[i+j]=='.'){
				count++;
			}
		}
		//printf("%d\n",count);
		if(count+countp<80){
			printf("%d, %d\n",i-100,score);
			break;
		}
		countp = count;
		count = 0;
	}
    return 0;

}