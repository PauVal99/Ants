#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Hawk



struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }


  /**
   * Types and attributes for your player can be defined here.
   */



  const vector<NutrientType> nutrients = { Carbohydrate, Protein, Lipid };


  // Default direction to be used during all match.
//  Dir default_dir;
typedef  vector<bool> VB;
typedef  vector<VB> VBB;
VBB ocupat;

  // Returns true if winning.
  bool winning() {
    for (int pl = 0; pl < num_players(); ++pl)
      if (pl != me() and score(me()) <= score(pl))
        return false;
    return true;
  }

 // ABLE TO MOVE WORKERS OR SOLDIERS
bool able_w(int id, Pos aux){
  return(pos_ok(aux) and cell(aux).type != Water and cell(aux).id == -1);
}

bool able_s(int id, Pos aux){
  return (pos_ok(aux) and cell(aux).type != Water and (ant(cell(aux).id).player != me()));
}



//   QUIN NURTIENT ES EL MINIM
  NutrientType min () {
    NutrientType n;
    vector<int> queen_id = queens(me());
    Ant queen = ant(queen_id[0]);
    int min = queen.reserve[Carbohydrate];
        if (min < queen.reserve[Protein] and  min < queen.reserve[Lipid] ) return Carbohydrate;
        else if (queen.reserve[Protein] < queen.reserve[Carbohydrate] and queen.reserve[Protein] < queen.reserve[Lipid] ) return Protein;
        else return Lipid;
  }




 // TRANSFORMA INT A DIRECCIĂ
  Dir transform (int x){
  if (x == 0) return Up;
  else if( x == 1) return Right;
  else if (x == 2) return Down;
  else return Left;
}

   //  LA REINA POT POSAR OUS?
    bool able_lay(int id, int k, string s){
      Ant queen = ant(id);
      for (NutrientType n : nutrients)
        if( s == "Worker"){
       if (queen.reserve[n] < 1) return false;
      }
      else{
        if (queen.reserve[n] < 3) return false;
      }
      Pos p = queen.pos + transform(k);
       if( pos_ok(p) and cell(p).id == -1 and cell(p).type != Water ) return true;
    return false;
    }

// ES POT MATAR A UN WORKER ENEMIC?

void able_kill(int id){
  Ant soldier = ant(id);
  Pos p = soldier.pos;
  for(int k = 0; k < 4; ++k){
    Pos aux = p + transform(k);
    if(((ant(cell(aux).id).player != me()) and ant(cell(aux).id).type == Worker) and able_s(id,aux)){
       move(id,transform(k));
       return;
     }
  }
}

bool able_kill_q(Pos p){
  return ((ant(cell(p).id).player != me()) and ant(cell(p).id).type != Queen  and cell(p).type != Water);
}


bool close_s(Pos p){
  vector <Pos> v;
  v.push_back(p);
  v.push_back(p+Up);
  v.push_back(p+Down);
  v.push_back(p+Left);
  v.push_back(p+Right);
  v.push_back(p+Up+Left);
  v.push_back(p+Up+Right);
  v.push_back(p+Down+Left);
  v.push_back(p+Down+Right);
  for(int i = 0; i < int(v.size()-1);++i){
    if(cell(v[i]).id != -1 and ant(cell(v[i]).id).type == Soldier and (ant(cell(v[i]).id).player != me())) return true;
  }
  return false;
}

bool close_q(Pos p){
  vector <Pos> v;
  v.push_back(p);
  v.push_back(p+Up);
  v.push_back(p+Down);
  v.push_back(p+Left);
  v.push_back(p+Right);
  v.push_back(p+Up+Left);
  v.push_back(p+Up+Right);
  v.push_back(p+Down+Left);
  v.push_back(p+Down+Right);
  for(int i = 0; i < int(v.size()-1);++i){
    if(ant(cell(v[i]).id).type == Queen and (ant(cell(v[i]).id).player == me())) return true;
  }
  return false;
}

bool close_f(Pos p){
  vector <Pos> v;
  v.push_back(p);
  v.push_back(p+Up);
  v.push_back(p+Down);
  v.push_back(p+Left);
  v.push_back(p+Right);
  v.push_back(p+Up+Left);
  v.push_back(p+Up+Right);
  v.push_back(p+Down+Left);
  v.push_back(p+Down+Right);
  for(int i = 0; i < int(v.size()-1);++i){
    if(cell(v[i]).bonus!= None) return true;
  }
  return false;
}

// ESTA LA REINA APROP?
bool queen_near(int id){
 Ant worker = ant(id);
 Pos p = worker.pos;
 for(int k = 0; k < 4; ++k){
   Pos aux = p + transform(k);
  if((pos_ok(aux) and ant(cell(aux).id).type == Queen and (ant(cell(aux).id).player == me()))) return true;
}
 return false;
}
bool queen_rly_near(int id){
 Ant worker = ant(id);
 Pos p = worker.pos;
 for(int k = 0; k < 4; ++k){
   for(int j = 0; j < 4; ++j){
     for(int i = 0; i < 4; ++i){
   Pos aux = p + transform(k);
   Pos aux2 = aux + transform(j);
    Pos aux3 = aux2 + transform(i);
  if((pos_ok(aux) and ant(cell(aux).id).type == Queen and (ant(cell(aux).id).player == me())) or
  (pos_ok(aux2) and ant(cell(aux2).id).type == Queen and (ant(cell(aux2).id).player == me()))or
  (pos_ok(aux3) and ant(cell(aux3).id).type == Queen and (ant(cell(aux3).id).player == me()))) return true;
}
  }
}
 return false;
}

bool food_near(int id){
 Ant worker = ant(id);
 Pos p = worker.pos;
 for(int k = 0; k < 4; ++k){
   for(int j = 0; j < 4; ++j){
     for(int i = 0; i < 4; ++i){
   Pos aux = p + transform(k);
   Pos aux2 = aux + transform(j);
    Pos aux3 = aux2 + transform(i);
  if((pos_ok(aux) and cell(aux).bonus!= None) or
  (pos_ok(aux2) and cell(aux2).bonus!= None)or
  (pos_ok(aux3) and cell(aux3).bonus!= None)) return true;
}
  }
}
 return false;
}


 // BFS PER MATAR WORKERS
void kill(int id){
  Ant soldier = ant(id);
  Pos p = soldier.pos;
  VBB vis(25,VB(25,false));
  vector< vector< Pos > > cami (25, vector<Pos> (25, {-1,-1}));
  queue<Pos> positions;

    positions.push(p);
    vis[p.i][p.j]=true;

    while(!positions.empty()){
      Pos act = positions.front();
      positions.pop();
      for(int k = 0; k < 4; k++){
        Pos aux = act + transform(k);
        if(pos_ok(aux) and !vis[aux.i][aux.j]){
          vis[aux.i][aux.j]=true;
          if(!ocupat[aux.i][aux.j] and cell(aux).type != Water){
            positions.push(aux);
            cami[aux.i][aux.j] = act;
            if(cell(aux).id != -1 and ant(cell(aux).id).player != me() and ant(cell(aux).id).type == Worker and !close_s(aux)){ // test
              while(aux != soldier.pos){
                if(cami[aux.i][aux.j] == soldier.pos){
                  Pos a;
                  for(int k=0; k<4;k++){
                    a = p + Dir(k);
                    if(a == aux){
                      move(id, Dir(k));
                      ocupat[a.i][a.j] = true;
                      return;
                    }
                  }
                }
                aux = cami[aux.i][aux.j];
              }
            }
          }
        }
      }
    }
}

/*bool no_suicide (int id) {
  Ant worker = ant(id);
  Pos p = worker.pos;
  for(int k = 0; k < 4; k++){
    for(int i = 0; i < 4; i++){
      Pos aux = p + transform(k);
      Pos aux2 = aux + transform(i);
      if(cell(aux2).id != -1 and aux2 != p){
         if(i > 1 ) i -= 2;
         else i += 2;
         move(id,transform(i));
         return true;
        }
    }
  }
  return false;
}*/


 void find(int id){
   Ant worker = ant(id);
   Pos p = worker.pos;
   VBB vis(25,VB(25,false));
   vector< vector< Pos > > cami (25, vector<Pos> (25, {-1,-1}));
   queue<Pos> positions;

     positions.push(p);
     vis[p.i][p.j]=true;

     while(!positions.empty()){
       Pos act = positions.front();
       positions.pop();
       for(int k = 0; k < 4; k++){
         Pos aux = act + transform(k);
         if(pos_ok(aux) and !vis[aux.i][aux.j]){
           vis[aux.i][aux.j]=true;
           if(!ocupat[aux.i][aux.j] and cell(aux).type != Water and !close_s(aux)){
             positions.push(aux);
             cami[aux.i][aux.j] = act;
             if(cell(aux).id == -1 and cell(aux).bonus != None){
               while(aux != worker.pos){
                 if(cami[aux.i][aux.j] == worker.pos){
                   Pos a;
                   for(int k=0; k<4;k++){
                     a = p + Dir(k);
                     if(a == aux){
                       move(id, Dir(k));
                       ocupat[a.i][a.j] = true;
                       return;
                     }
                   }
                 }
                 aux = cami[aux.i][aux.j];
               }
             }
           }
         }
       }
     }
 }
 void find_for_queen(int id){
   Ant worker = ant(id);
   Pos p = worker.pos;
   VBB vis(25,VB(25,false));
   vector< vector< Pos > > cami (25, vector<Pos> (25, {-1,-1}));
   queue<Pos> positions;

     positions.push(p);
     vis[p.i][p.j]=true;

     while(!positions.empty()){
       Pos act = positions.front();
       positions.pop();
       for(int k = 0; k < 4; k++){
         Pos aux = act + transform(k);
         if(pos_ok(aux) and !vis[aux.i][aux.j]){
           vis[aux.i][aux.j]=true;
           if(!ocupat[aux.i][aux.j] and cell(aux).type != Water){
             positions.push(aux);
             cami[aux.i][aux.j] = act;
             if(cell(aux).id == -1 and cell(aux).bonus != None){
               while(aux != worker.pos){
                 if(cami[aux.i][aux.j] == worker.pos){
                   Pos a;
                   for(int k=0; k<4;k++){
                     a = p + Dir(k);
                     if(a == aux){
                       move(id, Dir(k));
                       ocupat[a.i][a.j] = true;
                       return;
                     }
                   }
                 }
                 aux = cami[aux.i][aux.j];
               }
             }
           }
         }
       }
     }
 }
 void find_queen(int id){
   Ant worker = ant(id);
   Pos p = worker.pos;
   VBB vis(25,VB(25,false));
   vector< vector< Pos > > cami (25, vector<Pos> (25, {-1,-1}));
   queue<Pos> positions;

     positions.push(p);
     vis[p.i][p.j]=true;

     while(!positions.empty()){
       Pos act = positions.front();
       positions.pop();
       for(int k = 0; k < 4; k++){
         Pos aux = act + transform(k);
         if(pos_ok(aux) and !vis[aux.i][aux.j]){
           vis[aux.i][aux.j]=true;
           if(cell(aux).type != Water){
             positions.push(aux);
             cami[aux.i][aux.j] = act;
             if(cell(aux).id != -1 and ant(cell(aux).id).player == me() and ant(cell(aux).id).type == Queen){
               while(aux != worker.pos){
                 if(cami[aux.i][aux.j] == worker.pos and !ocupat[aux.i][aux.j]){
                   Pos a;
                   for(int k=0; k<4;k++){
                     a = p + Dir(k);
                     if(a == aux){
                       move(id, Dir(k));
                       ocupat[a.i][a.j] = true;
                       return;
                     }
                   }
                 }
                 aux = cami[aux.i][aux.j];
               }
             }
           }
         }
       }
     }
 }
   /**
    * Play method, invoked once per each round.
    */

  virtual void play () {
    ocupat = VBB(25,VB(25,false));
      for(int i = 0; i < 25; ++i){
        for(int j = 0; j < 25;++j){
          Pos p(i,j);
          if(cell(p).type == Water) ocupat[i][j] = true;
        }
      }

    vector<int> S = soldiers(me());
    vector<int> W = workers(me());
    vector<int> Q = queens(me());
    for(int id : S){
      Ant soldier = ant(id);
       Pos p = soldier.pos;
       ocupat[p.i][p.j] = true;
    }

   for (int id : W){
     Ant worker = ant(id);
     Pos p = worker.pos;
     ocupat[p.i][p.j] = true;
  }

    for(int id : Q){
      Ant queen = ant(id);
      Pos p = queen.pos;
      ocupat[p.i][p.j] = true;
    }
    for(int id : S){
      //able_kill(id);
      kill(id);
    }
    for (int id : W){
      Ant worker = ant(id);
      Pos p = worker.pos;
      if(worker.bonus == None){
          if(cell(p).bonus != None and !queen_rly_near(id)){
            take(id);
          }
          else{
            if(!queen_rly_near(id))
            find(id);
            else kill(id);
          }
        }
        else {
          if(close_q(p)) leave(id);
          find_queen(id);
      }
    }
    for(int id : Q){           // MOURE REINA
      if (round()%2 == 0) {
      Ant queen = ant(id);
      Pos p = queen.pos;
      ocupat[p.i][p.j] = true;
      for(int k = 0; k < 4; ++k){
        Pos aux = p + transform(k);
       if(close_s(aux)) move(id,transform(k));
        if(food_near(id))  find_for_queen(id);
        if(!close_s(p)){
        if(able_lay(id,k,"Worker")) lay(id,transform(k),Worker);
        }
      /*  else{
          if(able_lay(id,k,"Soldier")) lay(id,transform(k),Soldier);
        } */
      }
      find_for_queen(id);
    }
  }
}

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
