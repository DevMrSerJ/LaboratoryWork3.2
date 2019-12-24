#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

FILE * BinarFile; // Указатель на файл
char* nameFile = (char*)malloc(20); // название бинарного файла

struct InfoAnimal // Структура - информация о животных
{
	string name; // Название животного
	string naturalZone; // Природная зона
	int cost; // Затраты на корм в сутки 
};

void AddRecord(FILE *file, char* nameF)
{
	system("cls");

	file = fopen(nameF, "ab");

	InfoAnimal anim;

	string name = ""; // название
	string zone = ""; // природная зона
	int cost = 0; // затраты в сутки

	cin.get();
	cout << "Введите название животного: ";
	getline(cin, name);
	cout << "Введите природную зону обитания: ";
	getline(cin, zone);
	cout << "Введите затраты в день на корм данного животного: ";
	cin >> cost;

	// Создаем объект структуры и инициализируем его
	anim.name = name;
	anim.naturalZone = zone;
	anim.cost = cost;

	if (BinarFile != NULL)
	{
		fseek(BinarFile, 0, SEEK_END);
		fwrite(&anim, sizeof(struct InfoAnimal), 1, BinarFile);
	}

	fclose(file);
}

void DeleteRecord(FILE * file, char* name)
{
	system("cls");

	file = fopen(name, "rb");

	fseek(file, 0, SEEK_SET);

	cout << "Номер\tНазвание животного\tПриродная зона\t\tЗатраты" << endl;

	int cou;
	int numb;
	numb = 1;

	InfoAnimal anim;

	while (true)//пока не достигли конца файла
	{
		cou = fread(&anim, sizeof(struct InfoAnimal), 1, file);

		if (cou != 1)
			break;

		cout << " " + to_string(numb) + "\t  " + anim.name + "\t\t\t  " + anim.naturalZone + "\t\t\t" + to_string(anim.cost) << endl;

		numb++;
	}

	int num;

	while (true)
	{
		cout << "Введите номер записи, которую хотите удалить: ";

		cin >> num;

		if (num > numb-1 || num < 0)
			cout << "Такого номера нет, попробуйте снова..." << endl;
		else
			break;

		getchar();
		getchar();
	}

	int i;
	i = 0;

	while (i != num)
	{
		cou = fread(&anim, sizeof(struct InfoAnimal), 1, file);
		i++;
	}

	//cout << "\n\n " + to_string(i) + "\t  " + anim.name + "\t\t\t  " + anim.naturalZone + "\t\t\t" + to_string(anim.cost) << endl;

	fclose(file);
}

int main()
{
	// Установка кириллицы
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	system("cls");

	cout << "\t\tЗадание №4\n";

	cout << "Введите название файла: ";

	int i = 0;
	
	while (i < 20)
	{
		nameFile[i] = getchar();
		if (nameFile[i] == '\n')
			break;
		i++;
	}
	nameFile[i] = '\0';
	nameFile = (char*)realloc(nameFile, i + 1);

	BinarFile = fopen(nameFile, "w+b");
	fclose(BinarFile);
	
	bool funct = true;

	while (funct)
	{
		system("cls");
		cout << "\t\tЗадание №4\n";
		cout << "\n\tМеню базы животных: " << endl;
		cout << "1 - добавить запись" << endl;
		cout << "2 - удалить запись" << endl;
		cout << "3 - изменить запись" << endl;
		cout << "4 - количсетво животных из определенной зоны" << endl;
		cout << "5 - затраты на определнного животного в месяц" << endl;
		cout << "0 - выйти в меню" << endl;
		cout << "Введите номер задания: ";
		int numLesson;
		cin >> numLesson;

		InfoAnimal anim;

		string name = ""; // название
		string zone = ""; // природная зона
		int cost = 0; // затраты в сутки
		

		switch (numLesson)
		{
		case 1:
			AddRecord(BinarFile, nameFile);
			break;
		case 2:
			DeleteRecord(BinarFile, nameFile);
			getchar();
			getchar();
			break;
		case 0:
			funct = false;
			break;
		}
	}

	

	system("PAUSE");
}