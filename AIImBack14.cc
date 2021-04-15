#include "Player.hh"

#define PLAYER_NAME ImBack14

struct PLAYER_NAME : public Player {

  /**
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

 vector< vector<bool> > free_positions;
 vector< vector<bool> > food = vector< vector<bool> > (25, vector<bool>(25,false));
 int w,s;

  bool near_enemy(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me()){
        return true;
      }
    }
    return false;
  }

  bool near_ally_queen(Pos p){
    for(int k=0; k<4; k++){
      Pos adj = p + Dir(k);
      if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).type == Queen and ant(cell(adj).id).player == me()){
        return true;
      }
    }
    return false;
  }

  bool near_enemy_ant(Pos p, AntType a){
      for(int k=0; k<4; k++){
        Pos adj = p + Dir(k);
        if(pos_ok(adj) and cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type == a){
          return true;
        }
      }
      return false;
  }

  bool near_enemy_soldier(Pos p){
    return near_enemy_ant(p,Soldier);
  }

  bool near_enemy_queen(Pos p){
    return near_enemy_ant(p,Queen);
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

  bool search_food(Ant a){
    if(cell(a.pos).bonus != None and !near_ally_queen(a.pos)){
      take(a.id);
      return true;
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
            if(!near_ally_queen(adj) and cell(adj).bonus != None){
              path(P,a,a.pos,adj);
              food[adj.i][adj.j] = true;
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  void go_queen(Ant a){
    if(near_ally_queen(a.pos)){
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
            if(near_ally_queen(adj)){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  void go_food(Ant a){
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
            if(food[adj.i][adj.j]){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  bool avoid_enemies(Ant a){
    if(near_enemy(a.pos)){
      for(int k=0; k<4; k++){
        Pos adj = a.pos + Dir(k);
        if(pos_ok(adj) and free_positions[adj.i][adj.j] and cell(adj).type != Water and cell(adj).id == -1 and !near_enemy(adj)){
          move(a.id,Dir(k));
          free_positions[adj.i][adj.j] = false;
          return true;
        }
      }
    }
    return false;
  }

  void move_workers(){
    vector<int> W = workers(me());
    for(int id : W){
      Ant a = ant(id);
      if(avoid_enemies(a));
      else if(a.bonus == None){
        if(search_food(a));
        else go_food(a);
      }
      else go_queen(a);
    }
  }

  bool enemy_soldier_queen(Pos p){
    return cell(p).id != -1 and ant(cell(p).id).player != me() and ant(cell(p).id).type != Worker;
  }

  bool kill_workers(Ant a){
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
          if(free_positions[adj.i][adj.j] and cell(adj).type != Water and !enemy_soldier_queen(adj) and !near_enemy_soldier(adj) and !near_enemy_queen(adj)){
            Q.push(adj);
            P[adj.i][adj.j] = make_pair(p,Dir(k));
            if(cell(adj).id != -1 and ant(cell(adj).id).player != me() and ant(cell(adj).id).type == Worker){
              path(P,a,a.pos,adj);
              return true;
            }
          }
        }
      }
    }
    return false;
  }

  bool avoid_soldiers_queens(Ant a){
    if(near_enemy_soldier(a.pos) or near_enemy_queen(a.pos)){
      for(int k=0; k<4; k++){
        Pos adj = a.pos + Dir(k);
        if(pos_ok(adj) and free_positions[adj.i][adj.j] and cell(adj).type != Water and !enemy_soldier_queen(adj) and !near_enemy_soldier(adj) and !near_enemy_queen(adj)){
          move(a.id,Dir(k));
          free_positions[adj.i][adj.j] = false;
          return true;
        }
      }
    }
    return false;
  }

  void move_soldiers(){
    vector<int> S = soldiers(me());
    for(int id : S){
      Ant a = ant(id);
      if(avoid_soldiers_queens(a));
      else kill_workers(a);
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

  bool lay_solder_egg(Ant a){
    if((round() >= 100 and round() <= 140)){
      if(a.reserve[0] >= 3 and a.reserve[1] >= 3 and a.reserve[2] >= 3){
        for(int k=0;k<4;k++){
          Pos adj = a.pos + Dir(k);
          if(pos_ok(adj) and cell(adj).type != Water and cell(adj).id == -1){
            lay(a.id, Dir(k), Soldier);
            free_positions[adj.i][adj.j] = false;
            return true;
          }
        }
      }
    }
    return false;
  }

  bool lay_worker_egg(Ant a){
    if((round() >= 140)){
      if(a.reserve[0] >= 1 and a.reserve[1] >= 1 and a.reserve[2] >= 1){
        for(int k=0;k<4;k++){
          Pos adj = a.pos + Dir(k);
          if(pos_ok(adj) and cell(adj).type != Water and cell(adj).id == -1){
            lay(a.id, Dir(k), Worker);
            free_positions[adj.i][adj.j] = false;
            return true;
          }
        }
      }
    }
    return false;
  }

  void search_food_queen(Ant a){
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
          if(free_positions[adj.i][adj.j] and cell(adj).type != Water and !near_enemy_queen(adj)){
            Q.push(adj);
            P[adj.i][adj.j] = make_pair(p,Dir(k));
            if(cell(adj).bonus != None){
              path(P,a,a.pos,adj);
              return;
            }
          }
        }
      }
    }
  }

  void move_queen(){
    vector<int> Q = queens(me());
    for(int id : Q){
      Ant a = ant(id);
      if(kill(a)) return;
      else if(lay_solder_egg(a)) return;
      else if(eat(a)) return;
      else if(lay_worker_egg(a)) return;
      else if(w < 3) search_food_queen(a);
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
