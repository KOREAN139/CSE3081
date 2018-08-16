#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

typedef pair<unsigned int, int> p;

vector<p> merge(vector<p> P, int x, int idx)
{
	vector<p> L, T;
	for(int i = 0; i < P.size(); i++)
	{
		p tmp = P[i];
		tmp.first += x;
		tmp.second |= (1<<idx);
		T.push_back(tmp);
	}
	for(int i = 0, j = 0; i < P.size() || j < T.size(); )
		L.push_back(i<P.size()?j<T.size()?P[i].first<T[j].first?P[i++]:T[j++]:P[i++]:T[j++]);
	return L;
}

vector<p> trim(vector<p> P, double delta)
{
	p last = P[0];
	vector<p> L;
	L.push_back(last);
	for(int i = 1; i < P.size(); i++)
	{
		if(P[i].first > (double)last.first * (1+delta))
		{
			L.push_back(P[i]);
			last = P[i];
		}
	}
	return L;
}

p exact(int n, int L, vector<unsigned int> S)
{
	vector<p> P;
	P.push_back(p(0, 0));
	for(int i = 0; i < n; i++)
	{
		P = merge(P, S[i], i);
		for(int j = P.size()-1; P[j].first > L; j--) P.erase(P.begin()+j);
	}
	return P[P.size()-1];
}

p approx(int n, double ep, int L, vector<unsigned int> S)
{
	vector<p> P;
	P.push_back(p(0, 0));
	for(int i = 0; i < n; i++)
	{
		P = merge(P, S[i], i);
		P = trim(P, ep/(2*n));
		for(int j = P.size()-1; P[j].first > L; j--) P.erase(P.begin()+j);
	}
	return P[P.size()-1];
}

int main()
{
	int tc;
	FILE *fp, *tcfp;

	fp = fopen("SS_test_command.txt", "r");
	fscanf(fp, "%d", &tc);

	while( tc-- )
	{
		p A;
		int n;
		unsigned int L, exact_ans;
		char in[33], out_exact[33], out_approx[33];
		double ep;
		vector<unsigned int> S;

		fscanf(fp, "%s%s%s", in, out_exact, out_approx);
		tcfp = fopen(in, "r");
		fscanf(tcfp, "%d%lf", &n, &ep);
		// get n, epsilon
		for(int i = 0; i < n; i++)
		{
			unsigned int t;
			fscanf(tcfp, "%d", &t);
			S.push_back(t);
		}
		fscanf(tcfp, "%d", &L);
		// get elements of set S, L
		fclose(tcfp);

		A = exact(n, L, S);
		tcfp = fopen(out_exact, "w");
		queue<int> AQ;
		for(int i = 0; i < 32; i++)
			if(A.second & (1<<i))
				AQ.push(i);
		fprintf(tcfp, "%lu\n", AQ.size());
		while( !AQ.empty() )
		{
			fprintf(tcfp, "%d\n", AQ.front());
			AQ.pop();
		}
		fclose(tcfp);
		// exact part

		A = approx(n, ep, L, S);
		tcfp = fopen(out_approx, "w");
		for(int i = 0; i < 32; i++)
			if(A.second & (1<<i))
				AQ.push(i);
		fprintf(tcfp, "%lu\n", AQ.size());
		while( !AQ.empty() )
		{
			fprintf(tcfp, "%d\n", AQ.front());
			AQ.pop();
		}
		fclose(tcfp);
		// approx part
	}

	fclose(fp);
	return 0;
}
