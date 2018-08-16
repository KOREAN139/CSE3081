#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

typedef pair<int, double> p;
typedef vector<vector<p> > graph;

vector<bool> visit, in, cycle;
// for checking black node, grey node

int c_node;

void dfs(int c, int parent, graph& g, stack<int>& s, stack<int>& cycle)
{
	visit[c] = 1;
	in[c] = 1; // set c as grey node
	for(int i = 0; i < g[c].size() && c_node < 0; i++)
	{
		int next = g[c][i].first;
		if(visit[next]) // when connected node's color is grey or black
		{
			if(in[next]) // if color is grey, it's cycle!
			{
				c_node = next;
				cycle.push(c);
				return;
			}
			else continue; // if color is black, then it's ok
		}
		dfs(next, c, g, s, cycle);
		if(c_node != -1) // when cycle is deteced
		{
			c_node = c_node^c?c_node:(1<<30); // when the first node of cycle is pushed at stack, reset c_node as inf ( something big )
			cycle.push(c);
		}
	}
	in[c] = 0; // set c as balck node
	s.push(c); // insert current node at front of topologically sorted list
}

int main()
{
	int tc;
	FILE *fp;
	fp = fopen("DAG_SP_test_command.txt", "r");
	fscanf(fp, "%d", &tc);

	while( tc-- )
	{
		char input[33], output[33];
		fscanf(fp, "%s%s", input, output);

		FILE *tcfp;
		tcfp = fopen(input, "r");

		int n, m, source, dest;
		double total = 0;
		stack<int> s;
		fscanf(tcfp, "%d%d", &n, &m);
		// get n, m
		graph g(n); visit.resize(n); in.resize(n);
		visit.clear(); in.clear();
		c_node = -1;
		// g for given graph, s for topological sorted graph
		
		for(int i = 0; i < m; i++)
		{
			int u, v;
			double w;
			fscanf(tcfp, "%d%d%lf", &u, &v, &w);
			g[u].push_back(p(v, w));
		}
		fscanf(tcfp, "%d%d", &source, &dest);
		fclose(tcfp);
		// read the given input
		
		stack<int> cycle;
		for(int i = 0; i < n && cycle.empty(); i++) if(!visit[i]) dfs(i, -1, g, s, cycle);
		// do topological sort for all vertices ( for the case when the given graph have more than one SCC )
		// cycle.empty() is the condition for acyclic

		stack<int> ans;
		if( cycle.empty() )
		{
			vector<double> d(n, 1e+9);
			vector<int> p(n, -1);
			d[source] = 0;
			// d is for shortest weight, p is for tracing back
			while( !s.empty() )
			{
				int cur = s.top(); s.pop();
				for(int i = 0; i < g[cur].size(); i++)
				{
					int next = g[cur][i].first;
					double cost = g[cur][i].second;
					if(d[next] > d[cur] + cost)
					{
						d[next] = d[cur] + cost;
						p[next] = cur;
					}
				}
				// do update in topological order
			}
			total = d[dest] < 1e+9 ? d[dest] : 0;
			dest = d[dest] < 1e+9 ? dest : -1;
			while( dest != -1 ) ans.push(dest), dest = p[dest];
			// tracing back the path
		}
		// find shortest path when given graph is DAG

		tcfp = fopen(output, "w");
		if( cycle.empty() )
		{
			fprintf(tcfp, "1\n%lu %lf\n", ans.size(), total);
			while( !ans.empty() )
				fprintf(tcfp, "%d\n", ans.top()), ans.pop();
		} else
		{
			fprintf(tcfp, "-1\n%lu\n", cycle.size());
			while( !cycle.empty() )
				fprintf(tcfp, "%d\n", cycle.top()), cycle.pop();
		}
		fclose(tcfp);
		// write the result of given testcase

		graph().swap(g);
		stack<int>().swap(s);
		// deallocating the memory
	}

	fclose(fp);
	return 0;
}
