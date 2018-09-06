bool isValidCoor(pii coor,int n){
	//cout << "n: " << n << endl;
	int x=coor.first;
	int y=coor.second;
	if(x<0 || x>=n){
		return false;
	}
	if(y<0 || y>=n){
		return false;
	}
	return true;
}

int get_acc(vector<double>& pr){
	double tot_pr=0;
	for (int i=0;i<pr.size();i++){
		tot_pr+=pr[i];
	}
	int probInt=rand()%((int)tot_pr+1);
	double probDec=double(1)/double(rand()%(1000)+1);
	double prob=double(probInt)+probDec;
	double pr_ac=0;
	for (int i=0;i<pr.size();i++){
		if(pr_ac>prob){
			return i-1;
		}
		pr_ac+=pr[i];
	}
	return pr.size()-1;
}
