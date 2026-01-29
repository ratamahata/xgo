//---------------------------------------------------------------------------


#pragma hdrstop

#include <memory.h>
#include "Evaluator.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define linec 54
unsigned char line [2][linec][10]=
{  "XXXXX","88888",//0,1
   "_XXXX_","x_XXX_x","xx_XX_xx","xxx_X_xxx",//2
//  3: close opened 3
   "_8888_","Xooo__","__oooX","To_ooT","Too_oT","oToToTo",
   "oTTooTo","oTooTTo","oooTTTooo","*_ooo_*",
//  4: build closed 4
   "XXXX_","_XXXX","XXX_X","X_XXX","XX_XX",
//  5: build opened 3
   "_XXX__","__XXX_","_XX_X_","_X_XX_","X_X_X_X",
   "X_XX__X","X__XX_X","XX_X__XX","XX__X_XX",
//  9:  close 2 or closed 3
   "TTooo","oooTT","?oToo?","?ooTo?","oToTo","oTToo","ooTTo",
   "_TooT_","TTToo_+","+_ooTTT","ToToT_","_ToToT","_oTTo_",
//  10: build 2  or closed 3
   "_TxTT_","_TTxT_","~X__X~",
   "XXX__","__XXX","_XXX_","X_XX_","_XX_X","XX_X_","_X_XX",
//11: ToT  (for the 2nd move)
   "ToT",
};

Evaluator::Evaluator(SimplyNumbers *simplyGen, Hashtable *movesHash)
        : Cursor (simplyGen, movesHash) {
  for(int j=0; j<linec; j++)
   for(int i=0; i<10; i++)

    switch (line[0][j][i])
    { case '~': line[0][j][i] = 2; break;//space (no attack)
      case 'x': line[0][j][i] = 4; break;
      case 'o': line[0][j][i] = 8; break;
      case 'T': line[0][j][i] =34; break;//X or space
      case 'X': line[0][j][i] =36; break;//X or x
      case '8': line[0][j][i] =40; break;//X or o
      case '+': line[0][j][i] =20; break;//x or #
      case '*': line[0][j][i] =52; break;//X or x or #
      case '?': line[0][j][i] =54; break;//X or x or # or space
      case '_': line[0][j][i] =66; break;//space (attack)
    };
  memcpy(line[1][0],line[0][0],linec*10);

  for(int j=0; j<linec; j++)
   for(int i=0; i<10; i++)

    if ((line[1][j][i]&12) != 12)
      if (line[1][j][i]&4) line[1][j][i] += 4;
      else if (line[1][j][i]&8) line[1][j][i] -= 4;
}

//==============================================================================

inline bool Evaluator::comp(int x, int y, unsigned char c)
{ if ((x<0)||(y<0)||(x>=fsize)||(y>=fsize))
    return (c&16);
  char n = kl[x+y*fsize];
  if (n<2) n = 2;
  return (n&c);
}

//==============================================================================

bool Evaluator::scanlines(int BlNo, int &lines, int N)
{ static const int bl[18] = {0,1,2,6,16,21,30,6,21,30,43,53,54,9,11,30,53},
     vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}}, p2[4] = {1,2,4,8};
  int nvec,sdv,nline,c, x = N%fsize, y = N/fsize,
        id = count & 1;//todo check if negation is needed here

  for (nvec=0; nvec<4; nvec++) if (lines&(p2[nvec]))
    for (nline = bl[BlNo]; nline<bl[BlNo+1]; nline++)
      for (sdv=0; sdv<9; sdv++) if (line[id][nline][sdv]&32)
        for (c = 0; c<9; c++)
          if (c==sdv) goto skip; else if (!line[id][nline][c]) goto skip;
          else if (comp(x+vec[nvec][0]*(c-sdv),y+vec[nvec][1]*(c-sdv),line[id][nline][c]))
skip:     { if (c==8)
            { lines -= p2[nvec];
              return true;
          } } else break;
  return false;
};

//==============================================================================

// Вспомогательная функция для безопасного добавления атак
void addAttackIfEmpty(TNode* destNode, int cx, int cy) {
    // Проверка границ поля fsize
    if (cx >= 0 && cx < fsize && cy >= 0 && cy < fsize) {
        TMove atk = (TMove)(cx + cy * fsize);
        for (int i = 0; i < MAX_ATTACK; i++) {
            if (destNode->attaсks[i] == atk) return;
            if (destNode->attaсks[i] == 0) {
                destNode->attaсks[i] = atk;
                return;
            }
        }
    }
}

int Evaluator::scanlines(int BlNo, int &lines, int N, TNode *destNode) {
  static const int bl[18] = {0,1,2,6,16,21,30,6,21,30,43,53,54,9,11,30,53};
  static const int vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}};
  static const int p2[4] = {1,2,4,8};

  int x = N % fsize, y = N / fsize;
  int id = count & 1;
  int totalFound = 0;

  for (int nvec = 0; nvec < 4; nvec++) {
    if (lines & p2[nvec]) {
      for (int nline = bl[BlNo]; nline < bl[BlNo + 1]; nline++) {
        for (int sdv = 0; sdv < 9; sdv++) {
          if (line[id][nline][sdv] & 32) {
            bool match = true;
            int vx = vec[nvec][0], vy = vec[nvec][1];

            for (int c = 0; c < 9; c++) {
              if (c == sdv) continue;
              unsigned char pChar = line[id][nline][c];
              if (!pChar) break;
              if (!comp(x + vx * (c - sdv), y + vy * (c - sdv), pChar & 63)) {
                match = false; break;
              }
            }

            if (match) {
              totalFound++;
              lines -= p2[nvec];

              // --- СПЕЦИАЛЬНЫЙ АЛГОРИТМ ДЛЯ BLNO=10 (Шаблоны 0 и 1) ---
              if (BlNo == 10 && (nline == bl[10] || nline == bl[10] + 1)) {
                int xPos = -1;
                for (int c = 0; c < 9; c++) {
                  if (line[id][nline][c] == 4) { // Находим позицию старого камня 'x'
                    xPos = c; break;
                  }
                }

                if (xPos != -1) {
                  int dist = (xPos > sdv) ? (xPos - sdv) : (sdv - xPos);
                  int leftEnd = (xPos < sdv) ? xPos : sdv;
                  int rightEnd = (xPos > sdv) ? xPos : sdv;

                  if (dist == 1) { // ПЛОТНАЯ ПАРА
                    int emptyL = 0;
                    for (int i = 1; i <= 3; i++)
                      if (comp(x + vx*(leftEnd-sdv-i), y + vy*(leftEnd-sdv-i), 2)) emptyL++; else break;
                    if (emptyL >= 3) {
                      addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv-1), y + vy*(leftEnd-sdv-1));
                      addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv-2), y + vy*(leftEnd-sdv-2));
                    } else if (emptyL == 2) {
                      addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv-1), y + vy*(leftEnd-sdv-1));
                    }

                    int emptyR = 0;
                    for (int i = 1; i <= 3; i++)
                      if (comp(x + vx*(rightEnd-sdv+i), y + vy*(rightEnd-sdv+i), 2)) emptyR++; else break;
                    if (emptyR >= 3) {
                      addAttackIfEmpty(destNode, x + vx*(rightEnd-sdv+1), y + vy*(rightEnd-sdv+1));
                      addAttackIfEmpty(destNode, x + vx*(rightEnd-sdv+2), y + vy*(rightEnd-sdv+2));
                    } else if (emptyR == 2) {
                      addAttackIfEmpty(destNode, x + vx*(rightEnd-sdv+1), y + vy*(rightEnd-sdv+1));
                    }
                  }
                  else if (dist == 2) { // ОДИН ПРОМЕЖУТОК
                    addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv+1), y + vy*(leftEnd-sdv+1));
                    if (comp(x+vx*(leftEnd-sdv-1), y+vy*(leftEnd-sdv-1), 2) && comp(x+vx*(leftEnd-sdv-2), y+vy*(leftEnd-sdv-2), 2))
                      addAttackIfEmpty(destNode, x+vx*(leftEnd-sdv-1), y+vy*(leftEnd-sdv-1));
                    if (comp(x+vx*(rightEnd-sdv+1), y+vy*(rightEnd-sdv+1), 2) && comp(x+vx*(rightEnd-sdv+2), y+vy*(rightEnd-sdv+2), 2))
                      addAttackIfEmpty(destNode, x+vx*(rightEnd-sdv+1), y+vy*(rightEnd-sdv+1));
                  }
                  else if (dist == 3) { // ДВА ПРОМЕЖУТКА
                    addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv+1), y + vy*(leftEnd-sdv+1));
                    addAttackIfEmpty(destNode, x + vx*(leftEnd-sdv+2), y + vy*(leftEnd-sdv+2));
                  }
                }
              }
              else { // --- ОБЫЧНЫЙ АЛГОРИТМ ---
                for (int c = 0; c < 9; c++) {
                  unsigned char pChar = line[id][nline][c];
                  if (!pChar || c == sdv) continue;
                  if (pChar & 64)
                    addAttackIfEmpty(destNode, x + vx*(c-sdv), y + vy*(c-sdv));
                }
              }
              goto next_vector;
            }
          }
        }
      }
    }
    next_vector:;
  }
  return totalFound;
}

//==============================================================================
void Evaluator::rate(TNode *src, TNode *destNode, TMove move) { //fills {totalRating,x3,x4,o3,o4} of dest;
  static const int vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}};

  destNode->o2 = src->x2;
  destNode->o3 = src->x3;
  destNode->o4 = src->x4;
  destNode->x2 = src->o2 > 1 ? src->o2 - 1 : src->o2;
  destNode->x3 = src->o3;
  destNode->x4 = src->o4;

  int t = 15; // 'lines'

  if (destNode->o4 > 0)// build 5
    if (scanlines(0, t, move)) {
      destNode->rating = 32600;
      return;
    } else {
        //should never happen (?)
        t=t;
    }
  int ret = 0;
  bool c3my = false;
  if (destNode->x4 > 0)//close 4
    if (scanlines(1, t, move)) {
        --destNode->x4;
        c3my = 1;
      }

  if (destNode->o3 > 0) {//build opened 4
    int found4o = scanlines(2, t, move, destNode);
    if (found4o) {
      destNode->o4 += 2;
      --destNode->o3;
//TODO it is better to return with high rating here
//      destNode->rating = 28000;
//      return;
    }
  }
  bool c3 = true;
  while (destNode->x3&&c3) {//close 3
    int t0 = t;
    if (0 != (c3 = scanlines(3, t, move))) {
        --destNode->x3;
        int tdiff = t0-t;
        int nvec = tdiff == 1 ? 0
                : tdiff == 2 ? 1
                : tdiff == 4 ? 2
                : tdiff == 8 ? 3
                : -1;//should not happen
//        if (nvec < 0) {//Die!
//                nvec = nvec / 0;
//        }
        int x = move % 15, y = move / 15;
        bool inside = kl[x + vec[nvec][0] + 15*(y + vec[nvec][1])]
                == kl[x - vec[nvec][0] + 15*(y - vec[nvec][1])];
        if (!inside) {
                //closed 3 is built
                destNode->x2 += 12;
        }
        ret = -600;//this is helpful for some reason
      }
  }


//  if (count == 1) {
//    if (scanlines(11, t, move)) {//������ ���
//        destNode->rating = 200;
//    } else {
//        destNode->rating = 0;
//    }
//    return;
//  }

  // 1. Сканируем на четверки. Те направления, где нашли 4, удалятся из t.
  int found4 = scanlines(4, t, move, destNode);
  if (found4) {
    destNode->o4 += found4;
    for (int i = 0; i<found4; ++i)
        destNode->o2 -= destNode->o2 >= 12 ? 12 : destNode->o2;
    //destNode->o2 -= found4*10;
    if (c3my) c3my = 2;
  }

  // 2. Сканируем на тройки только в ОСТАВШИХСЯ направлениях.
  int found3 = scanlines(5, t, move, destNode);
  if (found3) {
    destNode->o3 += found3;
    for (int i = 0; i<found3; ++i)
        destNode->o2 -= destNode->o2 >= 10 ? 10 : destNode->o2;
  }

  while (destNode->x2 > 0 && scanlines(9, t, move)) {//close enemy's two
    destNode->x2 -= destNode->x2 >= 9 ? 8 : (destNode->x2+1)/2; //>= 2 ? 2 : destNode->x2 >= 1 ? 1 : 0;
  }

//  while (scanlines(10, t, move)) {//build 2
//    destNode->o2 += 10;
//  }

  int f2 = scanlines(10, t, move);
  if (f2) destNode->o2 += f2*10;

        double k2 = destNode->age > 12 ? 0.1
                : destNode->age > 10 ? 0.2
                : destNode->age > 7 ? 0.4
                : destNode->age > 4 ? 0.7
                : destNode->age > 2 ? 0.9
                : 1;

        if (t > 0) {
            double k1 = destNode->age > 14 ? 0.1
                : destNode->age > 12 ? 0.3
                : destNode->age > 10 ? 0.5
                : destNode->age > 8 ? 0.7
                : destNode->age > 6 ? 0.8
                : destNode->age > 4 ? 0.9
                : 1;

            while (scanlines(11, t, move)) {//ToT
                if (0 == (count&1)) {//TODO recheck this
                        ret += (int)(220*k1);
                }
                else {ret -= (int)(110*k1);}
            }
        }
        ret -= src->rating*k2;


//  //hz hz
//  if ((c3my==2)&&(dest->o3)) {
//    int i=0, n15 = 15;
//    for (int N = 0; N<fsize*fsize; ++N)
//      if (kl[N]&3) {
//         while (scanlines(2, n15, N)) ++i;
//         n15 = 15;
//      }
//    dest->o3 = i;
//  }

exit:

//begin check special cases
//  if (destNode->age==4 && destNode->o2 >= 25 && destNode->x2 <= 5) {
//        ret = 6026;
        //destNode->fixedRating = true;
  //} else
  if (destNode->age==5) {//todo recheck
        int m1 = history[1].move;
        int m2 = history[3].move;
        int m3 = move;
        if (m1/15 == m2/15 && m1/15 == m3/15 || m1%15 == m2%15 && m1%15 == m3%15) {
          ret = -6026;
          destNode->setFixedRating(true);
        }
  }
//end check special cases

  if (destNode->isFixedRating()) {}
  else if (destNode->x4) {
    ret = -32200;
  } else if (destNode->x3 > 0 && destNode->o4 == 0) {
    ret = -30000;
  } else {
    ret +=destNode->o4*4200
        + destNode->o3*3840
        + destNode->o2*84
        - destNode->x3*6000
        - destNode->x2*150
      /*- src->o4*42
        - src->o3*30
        - src->o2*2
        + src->x3*60
        + src->x2*3*/;

//    if (destNode->o4 > 0 && destNode->o3 > 0) {
//            ret += 1000;
//    }
  }
#undef c3my


  destNode->rating = ret;


};





