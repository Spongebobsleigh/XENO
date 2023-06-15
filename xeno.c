#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 17
/*カードの構造*/
typedef struct card {
  char name[64]; /*カード名*/
  int rank;      /*カードのrank*/
  int effect;    /*カードの効果*/
} Card;
/*山札 =deck*/
Card deck[N] = {{"少年", 1, 0},  {"少年", 1, 1},  {"兵士", 2, 2},
                {"兵士", 2, 3},  {"占師", 3, 4},  {"占師", 3, 5},
                {"乙女", 4, 6},  {"乙女", 4, 7},  {"死神", 5, 8},
                {"死神", 5, 9},  {"貴族", 6, 10}, {"賢者", 7, 11},
                {"賢者", 7, 12}, {"精霊", 8, 13}, {"精霊", 8, 14},
                {"皇帝", 9, 15}, {"英雄", 10, 16}};
/*手札*/
Card hand1[2];
Card hand2[2];


/*捨て札*/
Card discard1[N];
Card discard2[N];


/*山札からカードを引く*/
void random(int *s, int d) {
  srand(time(NULL));
  *s = rand() % (17 - d);
}
void draw(int n, int s, int *d, Card *h /*hand*/, Card *x /*deck[s]*/,
          Card *y /*deck[16-d]*/) {
  *h = *x;
  *x = *y;
  *d += 1;
}
/*hand[0]を使用するとき*/
void use1(Card *z /*discard[n-1]*/, Card *h0 /*hand[0]*/,
          Card *h1 /*hand[1]*/) {
  *z = *h0;
  *h0 = *h1;
}
/*hand[1]を使用するとき*/
void use2(Card *z /*discard[n-1]*/, Card *h1 /*hand[1]*/) { *z = *h1; }
//兵士
void two(int *t, Card *h0_op /*相手のhand[0]*/) {
  int r;
  getchar();
  printf("[捜査]\n");
  printf("rankを指定してください。：");
  scanf("%d", &r);
  if ((h0_op->rank) == r) {
    printf("一致。\n");
    if (t == 0 && r == 10) {
      printf("英雄の効果で相手が転生します。\n");
      t += 1;
    } else {
      printf("勝利しました。\n");
      exit(0);
    }
  } else {
    printf("不一致。ゲームを続行します。\n");
  }
}
//占い師
void three(Card *h0_op /*相手のhand[0]*/) {
  printf("[透視]\n");
  printf("相手の手札は%s(rank:%d)です。\n", h0_op->name, h0_op->rank);
}


//死神
void five(int n, int s, int *d, int *t, Card *eff, Card *dis,
          Card *h0_op /*相手のhand[0]*/, Card *h1_op /*相手のhand[1]*/,
          Card *z_op /*相手のdiscard*/, Card *x /*deck[s]*/,
          Card *y /*deck[16-d]*/) {
  printf("[疫病]\n");
  if (N - *d > 0) {
    draw(n, s, d, h1_op, x, y);
    printf("相手は手札を１枚引きました。\n");
    *eff = *h1_op;
    printf("相手の手札をランダムに１枚捨てます。Enterキーを押して下さい。\n");
    getchar();
    srand(time(NULL));
    s = rand() % 2;
    if (s == 0) {
      use1(z_op, h0_op, h1_op);
    } else {
      use2(z_op, h1_op);
    }
    if (z_op->rank == 10 && t == 0) {
      printf("英雄の効果で相手が転生します。\n");
      t += 1;
    } else {
      printf("%s(rank:%d)が捨てられました。\n", z_op->name, z_op->rank);
      *dis = *z_op;
    }
  }
}


//貴族
void six(Card *h0_op /*相手のhand[0]*/, Card *h0_me /*自分のhand[0]*/) {
  getchar();
  printf("[対決]\n");
  printf("二人で画面を共有しEnterキーを押してください。\n");
  getchar();
  printf("相手の手札は%s(rank:%d)\n", h0_op->name, h0_op->rank);
  printf("自分の手札は%s(rank:%d)\n", h0_me->name, h0_me->rank);
  if (h0_op->rank > h0_me->rank) {
    printf("敗北");
    exit(0);
  } else if (h0_op->rank < h0_me->rank) {
    printf("勝利");
    exit(0);
  } else {
    printf("引き分け");
    exit(0);
  }
}


//賢者
void seven(int *d) {
  printf("[閲覧]\n");
  printf("自分の山札のカードの内、3枚をランダムで表示。\n");
  getchar();
  if (16 - *d >= 3) {
    printf("%s(rank:%d)\n", deck[0].name, deck[0].rank);
    printf("%s(rank:%d)\n", deck[1].name, deck[1].rank);
    printf("%s(rank:%d)\n", deck[2].name, deck[2].rank);
  } else if (16 - *d == 2) {
    printf("山札が３枚より少ないので残りの山札を表示します。\n");
    printf("%s(rank:%d)\n", deck[0].name, deck[0].rank);
    printf("%s(rank:%d)\n", deck[1].name, deck[1].rank);
  } else if (16 - *d == 1) {
    printf("山札が３枚より少ないので残りの山札を表示します。\n");
    printf("%s(rank:%d)\n", deck[0].name, deck[0].rank);
  } else {
    printf("山札にカードは残っていません。\n");
  }
}


//精霊
void eight(Card *h0_me /*自分のhand[0]*/, Card *h0_op /*相手のhand[0]*/) {
  Card temp;
  printf("[交換]\n");
  printf("お互いの手札を交換します。\n");
  temp = *h0_me;
  *h0_me = *h0_op;
  *h0_op = temp;
  printf("%s(rank:%d)を渡して、%s(rank:%d)を貰った。\n", h0_op->name,
         h0_op->rank, h0_me->name, h0_me->rank);
}
//皇帝
void nine(int n, int s, int *d, Card *eff, Card *dis,
          Card *h0_op /*相手のhand[0]*/, Card *h1_op /*相手のhand[1]*/,
          Card *z_op /*相手のdiscard*/, Card *x /*deck[s]*/,
          Card *y /*deck[16-d]*/) {
  int w;
  printf("[公開処刑]\n");
  if (N - *d > 0) {
    draw(n, s, d, h1_op, x, y);
    printf("相手は手札を１枚引きました。\n");
    *eff = *h1_op;
    printf("相手の手札を見ます。Enterキーを押してください。\n");
    getchar();
    printf("相手の手札\n");
    printf("%s(rank%d)\n", h0_op->name, h0_op->rank);
    printf("%s(rank%d)\n", h1_op->name, h1_op->rank);
    printf("捨てる相手の手札を１枚選んでください。(1or2)\n");
    printf("%s(rank:%d)→1を入力してください。\n", h0_op->name, h0_op->rank);
    printf("%s(rank:%d)→2を入力してください。\n", h1_op->name, h1_op->rank);
    scanf("%d", &w);
    while (w < 1 || 2 < w) {
      puts("入力しなおして下さい。");
      scanf("%d", &w);
    }
    if (w == 1) {
      use1(z_op, h0_op, h1_op);
    } else {
      use2(z_op, h1_op);
    }
    if (z_op->rank == 10) {
      printf("英雄を選んだのであなたの勝利です。\n");
      exit(0);
    } else {
      printf("%s(rank:%d)が捨てられました。\n", z_op->name, z_op->rank);
      *dis = *z_op;
    }
  }
}


int main(void) {
  int i;
  int s;
  int d = 0; /*ドロー回数*/
  int j;
  int c;
  int w;
  int n = 0; /*ターン数*/
  int t = 0; /*転生回数*/
  Card eff;  /*カードの効果で引いたカード*/
  Card dis;  /*カードの効果で捨てられたカード*/


  printf("ゲームを開始します。\n");
  printf("先行のプレイヤーはEnterキーを押してください。\n");
  /*最初の手札引く*/
  if ((c = getchar()) == '\n') {
    printf("貴方はプレイヤー１です。\n");
    random(&s, d);
    draw(n, s, &d, &hand1[0], &deck[s], &deck[16 - d]);
    printf("貴方の手札は%s(rank:%d)です。\n", hand1[0].name, hand1[0].rank);
  }
  printf("Enterキーを押して次のプレイヤーに代わってください。\n");
  if ((c = getchar()) == '\n') {
    system("cls");
  }
  printf("後攻のプレイヤーはEnterキーを押してください。\n");
  if ((c = getchar()) == '\n') {
    printf("貴方はプレイヤー２です。\n");
    random(&s, d);
    draw(n, s, &d, &hand2[0], &deck[s], &deck[16 - d]);
    printf("貴方の手札は%s(rank:%d)です。\n", hand2[0].name, hand2[0].rank);
  }
  printf("Enterキーを押して次のプレイヤーに代わってください。\n");
  if ((c = getchar()) == '\n') {
    system("cls");
  }
  /*ゲーム開始*/
  for (n = 1; 17 - d > 0; n++) {
    printf("プレイヤー１の%dターン目です。\n\n", n);
    if (n != 1)
      printf("相手の捨て札:%s(rank%d)\n", discard2[n - 2].name,
             discard2[n - 2].rank);
    printf("残りの山札の枚数:%d\n", 17 - d);
    if (discard2[n - 2].rank == 3) {
      if (discard1[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した占師(rank3)"
               "の効果で自分の手札を見られた。\n");
      }
    }
    if (discard2[n - 2].rank == 4) {
      printf("相手が前のターンに出した乙女(rank4)"
             "の効果で相手へのカードの効果が無効化されます。\n");
    }
    if (discard2[n - 2].rank == 5) {
      if (discard1[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した死神(rank5)"
               "の効果で自分は%s(rank:%d)を引いて、手札の%s(rank:%d)"
               "が捨てられた。\n",
               eff.name, eff.rank, dis.name, dis.rank);
      }
    }
    if (discard2[n - 2].rank == 8) {
      if (discard1[n - 2].rank == 4)
        printf(" ");
      else
        printf("相手が前のターンに出した精霊(rank8)の効果で、%s(rank:%d)"
               "を渡して%s(rank:%d)を貰った。\n",
               hand2[0].name, hand2[0].rank, hand1[0].name, hand1[0].rank);
    }
    if (discard2[n - 2].rank == 9) {
      if (discard1[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した皇帝(rank9)"
               "の効果で自分は%s(rank:%d)を引いて、手札の%s(rank:%d)"
               "が捨てられた。\n",
               eff.name, eff.rank, dis.name, dis.rank);
      }
    }
    /*手札を引く*/
    printf("Enterキーを押してカードを引いてください。\n");
    if ((c = getchar()) == '\n') {
      random(&s, d);
      draw(n, s, &d, &hand1[1], &deck[s], &deck[16 - d]);
      printf("貴方が引いたカード：%s(rank:%d)\n\n", hand1[1].name,
             hand1[1].rank);
    }
    /*使うカードの選択*/
    printf("場に出すカードを選択して下さい。(1or2)\n");
    printf("%s(rank:%d)→1を入力してください。\n", hand1[0].name, hand1[0].rank);
    printf("%s(rank:%d)→2を入力してください。\n", hand1[1].name, hand1[1].rank);
  TEN2:
    scanf("%d", &w);
    while (w < 1 || 2 < w) {
      puts("入力しなおして下さい。");
      scanf("%d", &w);
    }
    if (w == 1) {
      if (hand1[0].rank == 10) {
        printf("英雄は出せません。もう一度選んでください。\n");
        goto TEN2;
      } else
        use1(&discard1[n - 1], &hand1[0], &hand1[1]);
    } else if (w == 2) {
      if (hand1[1].rank == 10) {
        printf("英雄は出せません。もう一度選んでください。\n");
        goto TEN2;
      } else
        use2(&discard1[n - 1], &hand1[1]);
    }
    if (discard1[n - 1].rank == 1) {
      printf("効果なし。\n");
    }
    if (discard1[n - 1].rank == 2) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else
        two(&t, &hand2[0]);
      if (t == 1 && d > 0) {
        random(&s, d);
        draw(n, s, &d, &hand2[0], &deck[s], &deck[16 - d]);
        printf("相手は転生して%s(rank%d)になりました。\n", hand2[0].name,
               hand2[0].rank);
      } else if (t == 1 && d == 0) {
        printf("山札がないため相手は転生できませんでした( ´∀｀ )\n");
        printf("勝利しました。\n");
        exit(0);
      }
    }
    if (discard1[n - 1].rank == 3) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else
        three(&hand2[0]);
    }
    if (discard1[n - 1].rank == 4) {
      printf("次のターンまで自分へのカードの効果を無効化します。\n");
    }
    if (discard1[n - 1].rank == 5) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else {
        random(&s, d);
        five(n, s, &d, &t, &eff, &dis, &hand2[0], &hand2[1], &discard2[N],
             &deck[s], &deck[16 - d]);
        if (t == 1 && d > 0) {
          random(&s, d);
          draw(n, s, &d, &hand1[0], &deck[s], &deck[16 - d]);
          printf("相手は転生して%s(rank%d)になりました。\n", hand2[0].name,
                 hand2[0].rank);
        } else if (t == 1 && d == 0) {
          printf("山札がないため相手は転生できませんでした( ´∀｀ )\n");
        }
      }
    }
    if (discard1[n - 1].rank == 6) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else
        six(&hand2[0], &hand1[0]);
    }
    if (discard1[n - 1].rank == 7) {
      seven(&d);
    }
    if (discard1[n - 1].rank == 8) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else
        eight(&hand1[0], &hand2[0]);
    }
    if (discard1[n - 1].rank == 9) {
      if (discard2[n - 2].rank == 4)
        printf("相手の守護の効果で発動できませんでした。\n");
      else {
        random(&s, d);
        nine(n, s, &d, &eff, &dis, &hand2[0], &hand2[1], &discard2[N], &deck[s],
             &deck[16 - d]);
      }
    }


    getchar();
    printf("enterキーを押して次のプレイヤーに代わってください。");
    if ((c = getchar()) == '\n') {
      system("cls");
    }
    printf("プレイヤー2の%dターン目です。\n\n", n);
    printf("相手の捨て札:%s(rank%d)\n", discard1[n - 1].name,
           discard1[n - 1].rank);
    printf("残りの山札の枚数:%d\n", 17 - d);
    if (discard1[n - 1].rank == 3) {
      if (discard2[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した占師(rank3)"
               "の効果で自分の手札を見られた。\n");
      }
    }
    if (discard2[n - 2].rank == 4) {
      printf("相手が前のターンに出した乙女(rank4)"
             "の効果で相手へのカードの効果が無効化されます。\n");
    }
    if (discard1[n - 1].rank == 5) {
      if (discard2[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した死神(rank5)"
               "の効果で自分は%s(rank:%d)を引いて、手札の%s(rank:%d)"
               "が捨てられた。\n",
               eff.name, eff.rank, dis.name, dis.rank);
      }
    }
    if (discard1[n - 1].rank == 8) {
      if (discard2[n - 2].rank == 4)
        printf(" ");
      else
        printf("相手が前のターンに出した精霊(rank8)の効果で、%s(rank:%d)"
               "を渡して%s(rank:%d)を貰った。\n",
               hand1[0].name, hand1[0].rank, hand2[0].name, hand2[0].rank);
    }
    if (discard1[n - 1].rank == 9) {
      if (discard2[n - 2].rank == 4) {
        printf(" ");
      } else {
        printf("相手が前のターンに出した皇帝(rank9)"
               "の効果で自分は%s(rank:%d)を引いて、手札の%s(rank:%d)"
               "が捨てられた。\n",
               eff.name, eff.rank, dis.name, dis.rank);
      }
    }


    if (17 - d > 0) {
      printf("Enterキーを押してカードを引いてください。\n");
      if ((c = getchar()) == '\n') {
        random(&s, d);
        draw(n, s, &d, &hand2[1], &deck[s], &deck[16 - d]);
        printf("貴方が引いたカード：%s(rank:%d)\n\n", hand2[1].name,
               hand2[1].rank);
      }
      /*使うカードの選択*/
      printf("場に出すカードを選択して下さい。(1or2)\n");
      printf("%s(rank:%d)→1を入力してください。\n", hand2[0].name,
             hand2[0].rank);
      printf("%s(rank:%d)→2を入力してください。\n", hand2[1].name,
             hand2[1].rank);
    TEN1:
      scanf("%d", &w);
      while (w < 1 || 2 < w) {
        puts("入力しなおして下さい。");
        scanf("%d", &w);
      }
      if (w == 1) {
        if (hand2[0].rank == 10) {
          printf("英雄は出せません。もう一度選んでください。\n");
          goto TEN1;
        } else
          use1(&discard2[n - 1], &hand2[0], &hand2[1]);
      } else if (w == 2) {
        if (hand2[1].rank == 10) {
          printf("英雄は出せません。もう一度選んでください。\n");
          goto TEN1;
        } else
          use2(&discard2[n - 1], &hand2[1]);
      }
      if (discard2[n - 1].rank == 1) {
        printf("効果なし。\n");
      }
      if (discard2[n - 1].rank == 2) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else
          two(&t, &hand1[0]);
        if (t == 1 && d > 0) {
          random(&s, d);
          draw(n, s, &d, &hand1[0], &deck[s], &deck[16 - d]);
          printf("相手は転生して%s(rank%d)になりました。\n", hand1[0].name,
                 hand1[0].rank);
        } else if (t == 1 && d == 0) {
          printf("山札がないため相手は転生できませんでした( ´∀｀ )\n");
        }
      }
      if (discard2[n - 1].rank == 3) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else
          three(&hand1[0]);
      }
      if (discard2[n - 1].rank == 4) {
        printf("次のターンまで自分へのカードの効果を無効化します。\n");
      }
      if (discard2[n - 1].rank == 5) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else {
          random(&s, d);
          five(n, s, &d, &t, &eff, &dis, &hand1[0], &hand1[1], &discard1[N],
               &deck[s], &deck[16 - d]);
          if (t == 1 && d > 0) {
            random(&s, d);
            draw(n, s, &d, &hand2[0], &deck[s], &deck[16 - d]);
            printf("相手は転生して%s(rank%d)になりました。\n", hand1[0].name,
                   hand1[0].rank);
          } else if (t == 1 && d == 0) {
            printf("山札がなくて相手は転生できませんでした( ´∀｀ )\n");
          }
        }
      }
      if (discard2[n - 1].rank == 6) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else
          six(&hand1[0], &hand2[0]);
      }
      if (discard2[n - 1].rank == 7) {
        seven(&d);
      }
      if (discard2[n - 1].rank == 8) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else
          eight(&hand2[0], &hand1[0]);
      }
      if (discard2[n - 1].rank == 9) {
        if (discard1[n - 1].rank == 4)
          printf("相手の守護の効果で発動できませんでした。\n");
        else {
          random(&s, d);
          nine(n, s, &d, &eff, &dis, &hand1[0], &hand1[1], &discard1[N],
               &deck[s], &deck[16 - d]);
        }
      }
    }
    getchar();
    printf("Enterキーを押して次のプレイヤーに代わってください。");
    if ((c = getchar()) == '\n') {
      system("cls");
    }
  }
  printf("山札がなくなりました。\n");
  printf("二人で画面を共有しEnterキーを押してください。\n");
  getchar();
  printf("プレイヤー１の手札は%s(rank:%d)です。\n", hand1[0].name,
         hand1[0].rank);
  printf("プレイヤー２の手札は%s(rank:%d)です。\n", hand2[0].name,
         hand2[0].rank);
  if (hand1[0].rank > hand2[0].rank) {
    printf("プレイヤー１の勝利です。");
  } else if (hand1[0].rank < hand2[0].rank) {
    printf("プレイヤー2の勝利です。");
  } else {
    printf("引き分け");
  }
  return 0;
}