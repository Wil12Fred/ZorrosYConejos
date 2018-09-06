struct grass{
	int t;
	int amount,it;
	bool hasNeighbor;
	grass (){amount=0;it=0;t=0;hasNeighbor=0;}
	grass operator--(int){
		amount=max(0,amount-1);
		return *this;
	}
	grass operator++(int){
		it++;
		if(it==t){
			it=0;
			amount+=hasNeighbor;
		}
		return *this;
	}
};
