#include "Player.hh"

#define PLAYER_NAME ImBack10

struct PLAYER_NAME : public Player {

  /**
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

 vector< vector<bool> > free_positions;
 int egg_counter = 0,w,s;

  bool my_queen_is_arround(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).type == Queen and ant(cell(adj).id).player == me()){
        return true;
      }
    }
    return false;
  }

  bool near_enemy(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me()){
        return true;
      }
    }
    return false;
  }

  bool near_enemy_soldier(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type == Soldier){
        return true;
      }
    }
    return false;
  }

  bool near_enemy_queen(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type == Queen){
        return true;
      }
    }
    return false;
  }

  void path(const vector< vector<pair<Pos,Dir>> >& P, Ant a, Pos init, Pos end){
    Pos p = end;
    while (P[p.i][p.j].first != init) {
      p = P[p.i][p.j].first;
    }
    move(a.id,P[p.i][p.j].second);
    free_positions[p.i][p.j] = false;
  }

  void init_pos(){
    free_positions = vector< vector<bool> > (25, vector<bool>(25,true));
    Ant a;
    vector<int> W = workers(me());
    w = W.size();
    for(int id : W){
      a = ant(id);
      free_positions[a.pos.i][a.pos.j]=false;
    }
    vector<int> S = soldiers(me());
    s = S.size();
    for(int id : S){
      a = ant(id);
      free_positions[a.pos.i][a.pos.j]=false;
    }
    vector<int> Q = queens(me());
    for(int id : Q){
      a = ant(id);
      free_positions[a.pos.i][a.pos.j]=false;
    }
  }

  void search_food(Ant a){
    if(cell(a.pos).bonus != None and !my_queen_is_arround(a.pos)){
      take(a.id);
      return;
    }

    Pos aux = {-1,-1};
    pair<Pos,Dir> init = make_pair(aux,Up);
    vector< vector<pair<Pos,Dir>> > P(25,vector<pair<Pos,Dir>>(25,init));

    vector< vector<bool> > V(25, vector<bool> (25, false));
    queue<Pos> Q;
    Q.push(a.pos);
    V[a.pos.i][a.pos.j] = true;
    while (!Q.empty()) {
      Pos p = Q.front(); Q.pop();
      for(int k=0;k<4;++k){
        Pos adj = p + Dir(k);
        if(pos_ok(adj) and !V[adj.i][adj.j]){
          V[adj.i][adj.j] = true;
          if(free_positions[adj.i][adj.j] and cell(adj).type != Water and cell(adj).id == -1 and !near_enemy(adj)){
            Q.push(adj);
            P[adj.i][adj.j] = make_pair(p,Dir(k));
            if(!my_queen_is_arround(adj) and cell(adj).bonus != None){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  void go_queen(Ant a){
    if(my_queen_is_arround(a.pos)){
      leave(a.id);
      return;
    }

    Pos aux = {-1,-1};
    pair<Pos,Dir> init = make_pair(aux,Up);
    vector< vector<pair<Pos,Dir>> > P(25,vector<pair<Pos,Dir>>(25,init));

    vector< vector<bool> > V(25, vector<bool> (25, false));
    queue<Pos> Q;
    Q.push(a.pos);
    V[a.pos.i][a.pos.j] = true;
    while (!Q.empty()) {
      Pos p = Q.front(); Q.pop();
      for(int k=0;k<4;++k){
        Pos adj = p + Dir(k);
        if(pos_ok(adj) and !V[adj.i][adj.j]){
          V[adj.i][adj.j] = true;
          if(free_positions[adj.i][adj.j] and cell(adj).type != Water and cell(adj).id == -1 and !near_enemy(adj)){
            Q.push(adj);
            P[adj.i][adj.j] = make_pair(p,Dir(k));
            if(my_queen_is_arround(adj)){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  void move_workers(){
    vector<int> W = workers(me());
    for(int id : W){
      Ant a = ant(id);
      a.bonus == None ? search_food(a) : go_queen(a);
    }
  }

  void kill_workers(Ant a){
    Pos aux = {-1,-1};
    pair<Pos,Dir> init = make_pair(aux,Up);
    vector< vector<pair<Pos,Dir>> > P(25,vector<pair<Pos,Dir>>(25,init));

    vector< vector<bool> > V(25, vector<bool> (25, false));
    queue<Pos> Q;
    Q.push(a.pos);
    V[a.pos.i][a.pos.j] = true;
    while (!Q.empty()) {
      Pos p = Q.front(); Q.pop();
      for(int k=0;k<4;++k){
        Pos adj = p + Dir(k);
        if(pos_ok(adj) and !V[adj.i][adj.j]){
          V[adj.i][adj.j] = true;
          if(free_positions[adj.i][adj.j] and cell(adj).type != Water and !near_enemy_soldier(adj) and !near_enemy_queen(adj)){
            Q.push(adj);
            P[adj.i][adj.j] = make_pair(p,Dir(k));
            if(cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type == Worker){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  void move_soldiers(){
    vector<int> S = soldiers(me());
    for(int id : S){
      Ant a = ant(id);
      kill_workers(a);
    }
  }

  bool kill(Ant a){
    for(int k=0;k<4;k++){
      Pos adj = a.pos + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type != Queen){
        move(a.id,Dir(k));
        free_positions[adj.i][adj.j] = false;
        return true;
      }
    }
    return false;
  }

  bool eat(Ant a){
    for(int k=0;k<4;k++){
      Pos adj = a.pos + Dir(k);
      if(pos_ok(adj) and cell(adj).bonus != None and cell(adj).id == -1){
        move(a.id,Dir(k));
        free_positions[adj.i][adj.j] = false;
        return true;
      }
    }
    return false;
  }

  bool lay_egg(Ant a){
    if(a.reserve[0] >= 1 and a.reserve[1] >= 1 and a.reserve[2] >= 1){
      for(int k=0;k<4;k++){
        Pos adj = a.pos + Dir(k);
        if(pos_ok(adj) and cell(adj).id == -1){
          if(egg_counter < 2){ lay(a.id, Dir(k), Worker); egg_counter++; return true;}
          else if (a.reserve[0] >= 3 and a.reserve[1] >= 3 and a.reserve[2] >= 3){ lay(a.id, Dir(k), Soldier); egg_counter = 0; return true;}
        }
      }
      return false;
    }
    else return false;
  }

  void move_queen(){
    vector<int> Q = queens(me());
    for(int id : Q){
      Ant a = ant(id);
      if(kill(a)) return;
      else if(eat(a)) return;
      else if(lay_egg(a)) return;
    }
  }

  virtual void play () {
    init_pos();
    if(round()%2 == 0) move_queen();
    move_workers();
    move_soldiers();
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
