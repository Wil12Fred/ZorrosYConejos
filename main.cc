#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <time.h>

using namespace std;

typedef pair<int,int> pii;

#include "utility.h"
#include "grass.h"
#include "animal.h"

struct conejo;
struct zorro;

struct conejo: animal{
	void act_pr(vector<grass*> GrassN){
		int movx[5]={0,0,-1,1,0};
		int movy[5]={1,-1,0,0,0};
		int pos=0;
		for (int i=0;i<5;i++){
			pii next=make_pair(coor.first+movx[i],
						coor.second+movy[i]);
			pr[i]=isValidCoor(next,n);
			if(pr[i] && i<4 && pos<GrassN.size()){
				if(GrassN[pos]->amount>0){
					pr[i]*=1.3;
				} else {
					pr[i]*=0.7;
				}
				pos++;
			}
		}
		if(it_sc==0)    pr[5]*=1.3;
		else	pr[5]*=0.8;
		//pr[5]=1;
	}
};

struct zorro: animal{
	void act_pr(vector<int> ConejoN){
		int movx[5]={0,0,-1,1,0};
		int movy[5]={1,-1,0,0,0};
		int pos=0;
		for (int i=0;i<5;i++){
			pii next=make_pair(coor.first+movx[i],
						coor.second+movy[i]);
			pr[i]=isValidCoor(next,n);
			if(pr[i] && i<4){
				if(ConejoN[pos]>0){
					pr[i]*=1.2;
				} else {
					pr[i]*=0.8;
				}
				pos++;
			}
		}
		if(it_sc==0)    pr[5]*=1.3;
		else	pr[5]*=0.9;
		//pr[5]=1;
	}
};

struct celda{
	pii coor;
	grass m_grass;
	vector<conejo*> *conejos, *forein_conejos;
	vector<zorro*> (*zorros), *forein_zorros;
	map<conejo*,bool> live_conejos;
	map<zorro*,bool> live_zorros;
	int ac_conejos;
	int ac_zorros;
	celda(){
		conejos=new vector<conejo*>();forein_conejos=new vector<conejo*>();
		zorros=new vector<zorro*>();forein_zorros=new vector<zorro*>();
	}
	void decidir(vector<celda*>& Neighbors){
		m_grass++;
		vector<grass*> GrassN;
		vector<int> ConejoN;
		for (int i=0;i<Neighbors.size();i++){
			GrassN.push_back(&Neighbors[i]->m_grass);
		}
		//cout << Neighbors.size() << endl;
		for (int i=0;i<Neighbors.size();i++){
			//cout << Neighbors[i]->conejos->size() << endl;
			ConejoN.push_back(Neighbors[i]->conejos->size());
		}
		for (int i=0;i<conejos->size();i++){
			(*conejos)[i]->animal::act_pr();
			//(*conejos)[i]->act_pr(GrassN);
			//cout << (*conejos)[i]->coor.first << " "
			//	<< (*conejos)[i]->coor.second << endl;
			(*((*conejos)[i]))++;
			if((*conejos)[i]->isLive()){
				if(((*conejos)[i])->comer){
				} else {
					//cout << "move: ";
					//cout << (*conejos)[i]->coor.first << " "
					//	<< (*conejos)[i]->coor.second << endl;
				}
				live_conejos[(*conejos)[i]]=0;
			} else {
				conejos->erase((*conejos).begin()+i);
				i--;
			}
		}
		for (int i=0;i<zorros->size();i++){
			(*zorros)[i]->animal::act_pr();
			//(*zorros)[i]->act_pr(ConejoN);
			//cout << (*zorros)[i]->coor.first << " "
			//	<< (*zorros)[i]->coor.second << endl;
			(*((*zorros)[i]))++;
			if((*zorros)[i]->isLive()){
				if(((*zorros)[i])->comer){
				} else {
					//cout << "move: ";
					//cout << (*zorros)[i]->coor.first << " "
					//	<< (*zorros)[i]->coor.second << endl;
				}
				live_zorros[(*zorros)[i]]=0;
			} else {
				zorros->erase(zorros->begin()+i);
				i--;
			}
		}
	}
	void ejecutar_conejo(int next,vector<celda*>& Neighbors ){
		int num_conejo=0;
		conejo* elegido=0;
		for (auto it=live_conejos.begin();it!=live_conejos.end();it++){
			if(!it->second) num_conejo++;
			if(num_conejo==next+1){
				elegido=it->first;
				break;
			}
		}
		if(elegido){
			if(elegido->comer){
				if(m_grass.amount>0){
					elegido->eat();
					m_grass--;
				}
			} else {
				for (int i=0;i<Neighbors.size();i++){
					if(elegido->coor==Neighbors[i]->coor){
						Neighbors[i]->forein_conejos->push_back(elegido);
						break;
					}
				}
			}
			live_conejos[elegido]=1;
		}
	}
	void ejecutar_zorro(int next,vector<celda*>& Neighbors ){
		int num_zorro=0;
		zorro* elegido=0;
		for (auto it=live_zorros.begin();it!=live_zorros.end();it++){
			if(!it->second) num_zorro++;
			if(num_zorro==next){
				elegido=it->first;
				break;
			}
		}
		if(elegido){
			if(elegido->comer){
				if( int(live_conejos.size()+forein_conejos->size()) ){
					elegido->eat();
					int presa=rand()%(int(live_conejos.size()+forein_conejos->size()));
					if(presa<live_conejos.size()){
						auto it=live_conejos.begin();
						for (int i=0;i<presa;i++){
							it++;
						}
						if(!it->second){
							ac_conejos--;
						}
						live_conejos.erase(it);
					} else {
						forein_conejos->erase(forein_conejos->begin()+(presa-live_conejos.size()));
					}
				}
			} else {
				for (int i=0;i<Neighbors.size();i++){
					if(elegido->coor==Neighbors[i]->coor){
						Neighbors[i]->forein_zorros->push_back(elegido);
					}
				}
			}
			live_zorros[elegido]=1;
		}
	}
	void ejecutar(vector<celda*>& Neighbors){
		ac_conejos=live_conejos.size();
		ac_zorros=live_zorros.size();
		while(ac_conejos+ac_zorros){
			int next=rand()%(ac_conejos+ac_zorros);
			if(next<ac_conejos){
				ejecutar_conejo(next,Neighbors);
				ac_conejos--;
			} else {
				ejecutar_zorro(next+1-ac_conejos,Neighbors);
				ac_zorros--;
			}
		}
	}
	void iterar(vector<celda*>& Neighbors){
		//Se actualiza parametros
		live_conejos.clear();
		live_zorros.clear();
		m_grass.hasNeighbor=false;
		for (int i=0;i<Neighbors.size() && m_grass.hasNeighbor==false;i++){
			if(Neighbors[i]->m_grass.amount>0){
				m_grass.hasNeighbor=true;
			}
		}
		// Se toma decisión
		decidir(Neighbors);
		// Realizar decision;
		ejecutar(Neighbors);
	}
	void actualizar(){
		conejos->clear();
		zorros->clear();
		for (auto it=live_conejos.begin();it!=live_conejos.end();it++){
			if(it->first->coor==coor){
				conejos->push_back(it->first);
			}
		}
		for (int i=0;i<forein_conejos->size();i++){
			conejos->push_back((*forein_conejos)[i]);
		}
		forein_conejos->clear();
		for (auto it=live_zorros.begin();it!=live_zorros.end();it++){
			if(it->first->coor==coor){
				zorros->push_back(it->first);
			}
		}
		for (int i=0;i<forein_zorros->size();i++){
			zorros->push_back((*forein_zorros)[i]);
		}
		forein_zorros->clear();
		int size_conejos=conejos->size();
		for (int i=0;i<size_conejos;i++){
			for (int j=0;j<(*conejos)[i]->desc.size();j++){
				animal* des=((*conejos)[i]->desc)[j] ;
				conejo* con_des=new conejo();
				con_des->coor=des->coor;
				con_des->t=des->t;
				con_des->v=des->v;
				con_des->d=des->d;
				con_des->n=des->n;
				conejos->push_back(con_des);
			}
			(*conejos)[i]->desc.clear();
		}
		int size_zorros=zorros->size();
		for (int i=0;i<size_zorros;i++){
			for (int j=0;j<(*zorros)[i]->desc.size();j++){
				animal* des=((*zorros)[i]->desc)[j] ;
				zorro* zor_des=new zorro();
				zor_des->coor=des->coor;
				zor_des->t=des->t;
				zor_des->v=des->v;
				zor_des->d=des->d;
				zor_des->n=des->n;
				zorros->push_back(zor_des);
			}
			(*zorros)[i]->desc.clear();
		}
	}
};

int main(){
	srand(time(NULL));
	int n=20;
	int grass_t=7;
	int conejo_t=5;
	int conejo_d=6;
	int conejo_v=4;
	int zorro_t=6;
	int zorro_d=2;
	int zorro_v=5;
	int iteraciones=30;
	celda Matriz[n][n];
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			Matriz[i][j].coor=make_pair(i,j);
			int agente=rand()%3;
			if(agente==0){
				Matriz[i][j].m_grass.amount=1;
				Matriz[i][j].m_grass.t=grass_t;
			} else if (agente==1){
				Matriz[i][j].m_grass.amount=1;
				Matriz[i][j].m_grass.t=grass_t;
				conejo* con_des=new conejo();
				con_des->coor=make_pair(i,j);
				con_des->t=conejo_t;
				con_des->d=conejo_d;
				con_des->v=conejo_v;
				con_des->n=n;
				Matriz[i][j].conejos->push_back(con_des);
			} else {
				zorro* zor_des=new zorro();
				zor_des->coor=make_pair(i,j);
				zor_des->t=zorro_t;
				zor_des->d=zorro_d;
				zor_des->v=zorro_v;
				zor_des->n=n;
				Matriz[i][j].zorros->push_back(zor_des);
			}
			int cant_conejos=Matriz[i][j].conejos->size();
		}
	}
	for (int k=0;k<iteraciones;k++){
		vector<pii> celdas_pendientes;
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				celdas_pendientes.push_back(make_pair(i,j));
			}
		}
		while(celdas_pendientes.size()){
			int next=rand()%celdas_pendientes.size();
			pii new_coor=celdas_pendientes[next];
			vector<celda*> Neighbors;
			int movx[4]={0,0,-1,1};
			int movy[4]={1,-1,0,0};
			for (int i=0;i<4;i++){
				pii neigh=make_pair(new_coor.first+movx[i],
							new_coor.second+movy[i]);
				if(isValidCoor(neigh,n)){
					Neighbors.push_back(&(Matriz[neigh.first][neigh.second]));
				}
			}
			Matriz[new_coor.first][new_coor.second].iterar(Neighbors);
			celdas_pendientes.erase(celdas_pendientes.begin()+next);
		}
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				Matriz[i][j].actualizar();
			}
		}
		int tot_grass=0;
		int tot_conejos=0;
		int tot_zorros=0;
		for (int i=0;i<n;i++){
			for (int j=0;j<n;j++){
				int cant_grass=Matriz[i][j].m_grass.amount;
				int cant_conejos=Matriz[i][j].conejos->size();
				int cant_zorros=Matriz[i][j].zorros->size();
				tot_grass+=cant_grass;
				tot_conejos+=cant_conejos;
				tot_zorros+=cant_zorros;
				if(cant_grass>max(cant_conejos,cant_zorros)){
					cout << "gr(" << cant_grass << ") ";
				} else if(cant_conejos>cant_zorros) {
					cout << "co(" << cant_conejos << ") ";
				} else {
					cout << "zo(" << cant_zorros << ") ";
				}
			}
			cout << endl;
		}
		cout << tot_grass << " " << tot_conejos << " " << tot_zorros << endl;
		cout << endl;
	}
	return 0;
}
