struct animal{
	int id;
	pii coor;
	int it_sc,t,v,d,n;
	int timeToReproduce;
	bool comer;
	int lastMove;
	vector<double> pr;
	vector<animal*> desc;
	animal (){it_sc=0;comer=0;pr.resize(6,1);lastMove=-1;}
	// Observar y Actualizar Probabilidades
	void act_pr(){
		int movx[5]={0,0,-1,1,0};
		int movy[5]={1,-1,0,0,0};
		for (int i=0;i<5;i++){
			pii next=make_pair(coor.first+movx[i],
						coor.second+movy[i]);
			pr[i]=isValidCoor(next,n);
		}
		//if(it_sc==0)	pr[5]*=1.5;
		pr[5]=1;
	}
	// Realizar una Acción
	animal operator++(int){
		if(it_sc>=v){
			kill();
			return *this;
		}
		timeToReproduce++;
		if(timeToReproduce>=t){
			for (int i=0;i<d;i++){
				desc.push_back(new animal());
				desc[desc.size()-1]->coor=coor;
				desc[desc.size()-1]->t=t;
				desc[desc.size()-1]->d=d;
				desc[desc.size()-1]->v=v;
				desc[desc.size()-1]->n=n;
			}
			timeToReproduce=0;
		}
		int acc=get_acc(pr);
		//cout << acc << endl;
		it_sc++;
		if(acc<5){
			if(acc<4){
				lastMove=acc;
			}
			comer=0;
			int movx[5]={0,0,-1,1,0};
			int movy[5]={1,-1,0,0,0};
			coor.first+=movx[acc];
			coor.second+=movy[acc];
			return *this;
		}
		comer=(acc==5);
		return *this;
	}
	// Morir
	void kill(){
		it_sc=-1;
	}
	// Comer
	void eat(){
		comer=false;
		it_sc=0;
	}
	// Estado del Animal
	bool isLive(){
		if(it_sc==-1){
			return false;
		}
		return true;
	}
};
