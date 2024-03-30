#include <stdio.h>
#include <stdlib.h>

int parent[1000000];

int root(int a)
{
	if (a == parent[a]) {
		return a;
	}

	return parent[a] = root(parent[a]);
}

void connect(int a, int b)
{
	a = root(a);
	b = root(b);

	if (a != b) {
		parent[b] = a;
	}
}

void connectedComponents(int n)
{
	int s[1000000] = {0}; 
	for (int i = 0; i < n; i++) {

		s[root(parent[i])] = 1;
	}

	int count = 0;
	for (int i = 0; i < n; i++) {
		if (s[i]) {
			count++;
		}
	}

	printf("%d\n", count);
}

void printAnswer(int N,
				int edges[][2],
				int M)
{

	for (int i = 0; i <= N; i++) {
		parent[i] = i;
	}

	for (int i = 0; i < M; i++) {
		connect(edges[i][0], edges[i][1]);
	}

	connectedComponents(N);
}

int main()
{
	int N = 8;

	int edges[][2] = {
		{ 1, 0 }, { 0, 2 }, { 5, 3 }, { 3, 4 }, { 6, 7 }
	};
	int M = sizeof(edges) / sizeof(edges[0]);

	printAnswer(N, edges, M);

	return 0;
}
