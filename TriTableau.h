#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include<thread>

using indiceInt = size_t;
using monType = double;

class TriTableau
{
	std::vector<monType> _tab; // Le tableau a trier
	std::vector<monType> _Sauf; // Un sauvegarde pour pouvoir faire plusieurs tris.
	std::vector<std::thread> tab_threads;
	std::vector<int>tab_index;
	inline void fusion(indiceInt deb1, indiceInt fin1, indiceInt deb2, indiceInt fin2);
	void init();

public:
	void reInit() { _tab = _Sauf; }

	void triMultiThreads(int nTread, std::string typeOfSort);// on passe en parametre le nombre de thread sur lequel on veut executer le tri et le type de tri.

	// Sort STL
	void triSTL();
	void triSTL_MT(indiceInt begin, indiceInt end);

	// QuickSort
	void swap(monType* a, monType* b);
	int partition(indiceInt l, indiceInt h);
	void quickSortIterative();
	void quickSortIterativeMT(indiceInt l, indiceInt h);

	// Debug
	bool isSort();
	void print();

	TriTableau() { init(); }
	indiceInt taille() { return _tab.size(); }
};

void TriTableau::init()
{
	indiceInt max;
	std::cout << "entrez un entier positif (en million)" << std::endl;
	std::cin >> max;
	max *= 1000000; // On multiplie par 1 million.
	if (max < 0)
		max *= -1; // On rend max positif au cas ou.
	else
		if (max == 0)
			max = 16; // Pour le debug au cas ou.


	_tab.resize(max);
	std::random_device rd;
	std::mt19937 re(rd());
	std::uniform_real_distribution<monType> dist(0, 99999); // Genere un nombre reel entre 0 et 99999.
	for (auto& it : _tab)
		it = dist(re); // On remplit le tableau de nombres aleatoires.
	_Sauf = _tab;

}

// Echange deux elements de place.
inline void TriTableau::swap(monType* a, monType* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

// Fonction pour le quickSort.
int TriTableau::partition(indiceInt l, indiceInt h)
{
	double x = _tab[h];
	indiceInt i = l;

	for (indiceInt j = l; j <= h - 1; j++)
	{
		if (_tab[j] < x)
		{
			std::swap(_tab[i], _tab[j]);
			i++;
		}
	}
	std::swap(_tab[i], _tab[h]);
	return (i);
}

/*	QuickSort pour le multi threads.
l --> Starting index,
h --> Ending index */
void TriTableau::quickSortIterativeMT(indiceInt l, indiceInt h)
{
	std::vector<monType> stack(h - l + 1);

	stack[0] = l;
	stack[1] = h;

	indiceInt top = 1;

	while (top < indiceInt(-1))
	{
		h = stack[top--];
		l = stack[top--];


		indiceInt p = partition(l, h);


		if (p > l + 1)
		{
			stack[++top] = l;
			stack[++top] = p - 1;
		}

		if (p + 1 < h)
		{
			stack[++top] = p + 1;
			stack[++top] = h;
		}
	}
}

// QuickSort pour le mono thread.
void TriTableau::quickSortIterative() {
	quickSortIterativeMT(0, _tab.size());
}

// Fonction principal qui va gérer le tri en multi threads.
inline void TriTableau::triMultiThreads(int nTread, std::string typeOfSort)
{
	if (nTread >= 2)
	{
		tab_threads.clear();
		indiceInt sizePerThread = _tab.size() / nTread; // Calcul de la taille du vecteur que devra trier chaque thread.

		// Calcul des index:
		tab_index.clear();
		int index = 0;
		for (int i = 0; i < nTread; ++i) {
			indiceInt begin = sizePerThread * i;
			indiceInt end = (sizePerThread * i + sizePerThread);
			tab_index.push_back(begin);
			tab_index.push_back(end);
		}

		if (typeOfSort == "quick") { // S'il s'agit du QuickSort.
			// On va répartir sur chaque thread une partie du vector a trié.
			for (int i = 0; i < nTread; ++i, index += 2) {
				tab_threads.push_back(std::thread(&TriTableau::quickSortIterativeMT, this, tab_index[index], tab_index[index + 1] - 1));
				
			}
		}
		else if (typeOfSort == "stl") {// S'il s'agit du sort qui provient de STD.
			for (int i = 0; i < nTread; ++i, index += 2) {
				tab_threads.push_back(std::thread(&TriTableau::triSTL_MT, this, tab_index[index], tab_index[index + 1]));
			}
		}
		else {
			std::cout << "aucun type de tri trouvé";
		}
		// On attend la fin de chaque thread
		for (auto& t : tab_threads) {
			t.join();
		}
		// Fusion des differentes parties triées en un seul vecteur trié.
		if (nTread == 2) {
			// Si le tableau est seulement divisé par deux on va juste fusionner ces deux parties en monothread.
			std::cout << "Pour 2 thread: " << std::endl;
			fusion(0, sizePerThread - 1, sizePerThread, sizePerThread * 2 - 1);
		}
		if (nTread == 4)
		{
			std::cout << "Pour 4 thread: " << std::endl;

			// On vient assigner la fusion de la première partie de la list et de la deuxième dans un thread.
			std::thread t5([&]()
				{
					fusion(tab_index[0], tab_index[1] - 1, tab_index[2], tab_index[3] - 1);
				});
			// On vient assigner la fusion de la troisième partie de la list et de la dernière dans un second thread.
			std::thread t6([&]()
				{
					fusion(tab_index[4], tab_index[5] - 1, tab_index[6], tab_index[7] - 1);
				});
			// On attend la fin de chaque thread
			t5.join();
			t6.join();
			// Dernière fusion afin de joindre les deux derniers blocks (en Mono Thread).
			fusion(0, sizePerThread * 2 - 1, sizePerThread * 2, sizePerThread * 4 - 1);
		}
	}
}



inline void TriTableau::triSTL_MT(indiceInt begin, indiceInt end)
{
	sort(_tab.begin() + begin, _tab.begin() + end);
}

inline void TriTableau::triSTL()
{
	sort(_tab.begin(), _tab.end());
}

inline  void TriTableau::fusion(indiceInt deb1, indiceInt fin1, indiceInt deb2, indiceInt fin2)
{

	if (fin1 + 1 == deb2)
	{
		indiceInt i = deb1;
		indiceInt j = deb2;
		indiceInt fin = fin1 - deb1 + fin2 - deb2;
		std::vector<monType> temp(fin + 2); // va contenir la fusion des 2 sous tableaux
		indiceInt k = 0;
		while (i <= fin1 && j <= fin2) // tant qu il reste des elements dans les 2 sous tableaux
		{
			if (_tab[i] < _tab[j]) // On met dans le tableau de fusion le plus petit et on incremente les indices
			{
				temp[k] = _tab[i];
				i++; k++;
			}
			else
			{
				temp[k] = _tab[j];
				j++; k++;
			}
		}
		for (; i <= fin1; i++, k++) // ATTENTION ici une de ces 2 boucles est vide suivant qu on soit sorti du 1er ou du 2em sous tableau
			temp[k] = _tab[i];
		for (; j <= fin2; j++, k++)
			temp[k] = _tab[j];

		// ensuite on recopie le vecteur de fusion :temp dans _tab

		for (k = deb1; k <= fin1; k++)
		{
			_tab[k] = temp[k - deb1];
		}

		for (k = deb2; k <= fin2; k++)
		{
			_tab[k] = temp[k - deb1];
		}
	}
}

// Verifie que le tableau est bien trie (ordre croissant)
bool TriTableau::isSort()
{
	float nberror = 0;
	for (indiceInt i = 0; i < _tab.size() - 1; ++i) {
		if (_tab[i] > _tab[i + 1]) { // si l'element actuel est superieur au suivant 
			nberror += 1;
		}
	}
	std::cout << "il y a eu " << (nberror / (float)_tab.size()) * 100 << " % d'erreur" << std::endl;
	if (nberror == 0) {
		return true;
	}
	else {
		return false;
	}
}

// Affiche les 16 elements du millieu du tableau 
void TriTableau::print()
{
	std::cout << "les 16 elements du millieu tableau : " << std::endl;
	indiceInt millieu = _tab.size() / 2;
	for (indiceInt i = millieu; i < millieu + 16; ++i) {
		std::cout << _tab[i] << " / ";
	}
	std::cout << std::endl;
}