#include <map>
#include <optional>
#include <limits>

template <typename T>
class queue_priority{
	private:
	std::map<T,int> l;
	public:
	queue_priority() {}
	
	bool empty() const{return l.empty();}
	
	//ha complessita O(n) ma era facile da scrivere
	T get(){
		int min = std::numeric_limits<int>::max();
		T best;
		for(const auto& [chiave, valore] : l)
		{
			if(valore<min)
			{
				best = chiave;
				min = valore;	
			}
			
		}
		l.erase(best);
		return best;}
		
	queue_priority& put(const T& elemento, const int valore) {
		l[elemento] = valore;
		return *this;} 
};