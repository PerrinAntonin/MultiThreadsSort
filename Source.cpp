
#include <iostream>
#include <chrono>

#include "TriTableau.h"

using namespace std;

// Driver code 
int main()
{
	TriTableau monTest;
	cout << "Initialisation : done" << endl;
	monTest.print(); // pour le debug

	cout << "Initialisation multi thread : done" << endl;
	std::cout << std::endl << std::endl << "   ###### Debut Test en monoThread ######" << std::endl;
	std::cout << std::endl << "-------  Tri quickSort ------" << std::endl << std::endl;

	// Calcul du temps pour le quicksort en monothread
	auto debut = std::chrono::system_clock::now();
	monTest.quickSortIterative();
	auto fin = std::chrono::system_clock::now();
	auto tempsMonoQuickSort = fin - debut;
	size_t tMonoQuickSort = std::chrono::duration_cast<std::chrono::milliseconds>(tempsMonoQuickSort).count();
	//Vérification du tri
	if (monTest.isSort()) {
		cout << "Temps execution tri quicksort monoThread: " << tMonoQuickSort << std::endl;
	}

	std::cout << std::endl << "-------  Tri stl ------" << std::endl << std::endl;	
	monTest.reInit();
	// Calcul du temps pour le tri std en monothread
	debut = std::chrono::system_clock::now();
	monTest.triSTL();
	fin = std::chrono::system_clock::now();
	auto tempsSortSTL = fin - debut;
	size_t tSTL = std::chrono::duration_cast<std::chrono::milliseconds>(tempsSortSTL).count();
	// Vérification du tri
	if (monTest.isSort())
		cout << "Temps execution tri STL monoThread: " << tSTL << endl;


	std::cout << std::endl << std::endl << "  ###### Debut Test en multitreads #######" << std::endl;

	std::cout << std::endl << "------- Debut Test QuickSort sort multitreads ------" << std::endl << std::endl;
	for (size_t nTread = 2; nTread <= 4; nTread += 2)
	{
		monTest.reInit();
		// Calcul du temps pour le tri quicksort en multiTread dabord avec 2 threads puis avec 4 threads
		debut = std::chrono::system_clock::now();
		monTest.triMultiThreads(nTread, "quick");
		fin = std::chrono::system_clock::now();
		auto tempsMulti = fin - debut;
		size_t tMulti = std::chrono::duration_cast<std::chrono::milliseconds>(tempsMulti).count();
		// Vérification du tri
		if (monTest.isSort())
			cout << "Temps execution multithread : " << tMulti << endl;
		else
			cout << "Tu te plantes en multi" << endl;

		cout << "Votre ratio  est de " << double(tMonoQuickSort) / tMulti << " theorique " << nTread << endl;
	}

	std::cout << std::endl << "------- Debut Test STD sort multitreads ------" << std::endl << std::endl;

	for (int nTread = 2; nTread <= 4; nTread += 2)
	{
		monTest.reInit();
		// Calcul du temps pour le tri stl en multiTread dabord avec 2 threads puis avec 4 threads
		debut = std::chrono::system_clock::now();
		monTest.triMultiThreads(nTread, "stl");
		fin = std::chrono::system_clock::now();
		auto tempsMulti = fin - debut;
		indiceInt tMulti = std::chrono::duration_cast<std::chrono::milliseconds>(tempsMulti).count();
		// Vérification du tri
		if (monTest.isSort())
			cout << "Temps execution multithread : " << tMulti << endl;
		else
			cout << "Tu te plantes en multi" << endl;

		cout << "Votre ratio  est de " << double(tSTL) / tMulti << " theorique " << nTread << endl;
	}
	return 0;
}


