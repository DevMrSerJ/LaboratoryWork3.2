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
	char* name; // Название животного
	char* naturalZone; // Природная зона
	int cost; // Затраты на корм в сутки 
};

int PrintRecords(FILE * file, char* nameF)
{
	file = fopen(nameF, "rb");

	int numb = 1;

	InfoAnimal anim;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)//пока не достигли конца файла
	{
		size_t cou = fread(&anim, sizeof(InfoAnimal), 1, file);

		if (cou != 1)
			break;

		cout << " " + to_string(numb) + "\t  " + anim.name + "\t\t\t  " + anim.naturalZone + "\t\t\t" + to_string(anim.cost) << endl;

		numb++;
	}

	fclose(file);

	return numb;
}

void AddRecord(FILE *file, char* nameF)
{
	system("cls");

	InfoAnimal anim;
	bool exist = false;// Есть ли данное животное в базе

	string name = ""; // название
	string zone = ""; // природная зона
	int cost = 0; // затраты в сутки

	cin.get();
	cout << "Введите название животного: ";
	getline(cin, name);

	file = fopen(nameF, "rb");

	InfoAnimal animal;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		bool coincidence = false;
		fread(&anim, sizeof(InfoAnimal), 1, file);

		if (strlen(anim.name) == name.length())
		{
			int i;
			for (i = 0; i < name.length(); i++)
			{
				if (tolower(anim.name[i]) != tolower(name[i]))
				{
					i = name.length() + 9;
				}
			}
			if (i != name.length() + 10)
				coincidence = true;
		}

		if (coincidence)
		{
			exist = true;
			break;
		}
	}
	fclose(file);

	cout << "Введите природную зону обитания: ";
	getline(cin, zone);
	cout << "Введите затраты в день на корм данного животного: ";
	cin >> cost;

	if (!exist)
	{
		// Создаем объект структуры и инициализируем его
		anim.name = (char*)malloc(name.length() + 2);

		for (int i = 0; i < name.length(); i++)
		{
			anim.name[i] = name[i];
		}
		anim.name[name.length()] = '\0';

		anim.naturalZone = (char*)malloc(zone.length() + 2);;

		for (int i = 0; i < zone.length(); i++)
		{
			anim.naturalZone[i] = zone[i];
		}
		anim.naturalZone[zone.length()] = '\0';

		anim.cost = cost;

		file = fopen(nameF, "ab");

		if (file != NULL)
		{
			fseek(file, 0, SEEK_END);
			fwrite(&anim, sizeof(InfoAnimal), 1, file);
		}

		fclose(file);
	}
	else
	{
		cout << "Данное животное уже записано в базу!";
	}

	cout << "Запись добавлена в базу. Чтобы выйти в меню нажмите Enter...";
	cin.get();
	getchar();
}

void DeleteRecord(FILE * file, char* name)
{
	system("cls");

	file = fopen(name, "rb");

	fseek(file, 0, SEEK_SET);

	cout << "Номер\tНазвание животного\tПриродная зона\t\tЗатраты" << endl;

	InfoAnimal anim;

	int numb = PrintRecords(file, name);

	int num;

	while (true)
	{
		cout << "Введите номер записи, которую хотите удалить: ";

		cin >> num;

		if (num > numb-1 || num < 1)
			cout << "Такого номера нет, попробуйте снова..." << endl;
		else
			break;

		getchar();
		getchar();
	}

	// Создаем новый бинарный файл
	FILE * fileRes = fopen("res.dat", "wb");
	
	int i = 0;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		fread(&anim, sizeof(InfoAnimal), 1, file);
		i++;

		if (i != num)// Если номер записи не совпадает с выбранной пользователем, записываем в новый файл
		{
			fseek(fileRes, 0, SEEK_END);
			fwrite(&anim, sizeof(InfoAnimal), 1, fileRes);
		}
	}

	fclose(fileRes);
	fclose(file);

	// Переименовываем и удаляем старый бинарный файл
	remove(name);
	rename("res.dat", name);

	cout << "Запись удалена. Чтобы выйти в меню нажмите Enter...";
	cin.get();
	getchar();
}

void EditRecord(FILE * file, char* name)// Изменить запись
{
	system("cls");

	file = fopen(name, "rb");

	int numb = PrintRecords(file, name);

	int num;

	while (true)
	{
		cout << "Введите номер записи, которую хотите изменить: ";

		cin >> num;

		if (num > numb - 1 || num < 1)
			cout << "Такого номера нет, попробуйте снова..." << endl;
		else
			break;

		cin.get();
		getchar();
	}

	int numStruct;

	while (true)
	{
		cout << "1 - название животного;" << endl;
		cout << "2 - зона обитания животного;" << endl;
		cout << "3 - затраты на животное в сутки;" << endl;

		cout << "Введите номер пункта, который хотите изменить: ";

		cin >> numStruct;

		if (numStruct > 3 || numStruct < 1)
			cout << "Такого номера нет, попробуйте снова..." << endl;
		else
			break;

		cin.get();
		getchar();
	}

	InfoAnimal anim;

	int i = 0;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		fread(&anim, sizeof(InfoAnimal), 1, file);
		i++;

		if (i == num)// Если номер записи не совпадает с выбранной пользователем, записываем в новый файл
		{
			break;
		}
	}

	string nameAnim = "";
	string zoneAnim = "";
	int costAnim = 0;

	switch (numStruct)
	{
	case 1:
		cout << "Введите название животного: ";
		cin >> nameAnim;
		anim.name = (char*)malloc(nameAnim.length() + 2);

		for (int i = 0; i < nameAnim.length(); i++)
		{
			anim.name[i] = nameAnim[i];
		}
		anim.name[nameAnim.length()] = '\0';
		break;
	case 2:
		cout << "Введите название зоны обитания животного: ";
		cin >> zoneAnim;
		anim.naturalZone = (char*)malloc(zoneAnim.length() + 2);

		for (int i = 0; i < zoneAnim.length(); i++)
		{
			anim.naturalZone[i] = zoneAnim[i];
		}
		anim.naturalZone[zoneAnim.length()] = '\0';
		break;
	case 3:
		cout << "Введите затрты на животное: ";
		cin >> costAnim;
		anim.cost = costAnim;
		break;
	}

	// Создаем новый бинарный файл
	FILE * fileRes = fopen("res.dat", "wb");

	InfoAnimal animal;
	i = 0;

	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		fread(&animal, sizeof(InfoAnimal), 1, file);
		i++;

		if (i != num)// Если номер записи не совпадает с выбранной пользователем, записываем в новый файл
		{
			fseek(fileRes, 0, SEEK_END);
			fwrite(&animal, sizeof(InfoAnimal), 1, fileRes);
		}
		else
		{
			fseek(fileRes, 0, SEEK_END);
			fwrite(&anim, sizeof(InfoAnimal), 1, fileRes);
		}
	}

	fclose(fileRes);
	fclose(file);

	// Переименовываем и удаляем старый бинарный файл
	remove(name);
	rename("res.dat", name);

	cout << "Запись изменена. Чтобы выйти в меню нажмите Enter...";
	cin.get();
	getchar();
}

void NumbAnimal(FILE * file, char* name)
{
	system("cls");

	file = fopen(name, "rb");

	string zoneAnim = "";

	cout << "Введите название зоны обитания: ";
	cin >> zoneAnim;

	InfoAnimal anim;
	int numb = 0;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		bool coincidence = false;
		fread(&anim, sizeof(InfoAnimal), 1, file);

		if (strlen(anim.naturalZone) == zoneAnim.length())
		{
			int i;
			for (i = 0; i < zoneAnim.length(); i++)
			{
				if (tolower(anim.naturalZone[i]) != tolower(zoneAnim[i]))
				{
					i = zoneAnim.length() + 9;
				}
			}
			if (i != zoneAnim.length() + 10)
				coincidence = true;
		}

		if (coincidence)
			numb++;
	}

	cout << "Количество животных данной природной зоны: " + to_string(numb) << endl;

	fclose(file);

	cout << "Чтобы выйти в меню нажмите Enter...";
	cin.get();
	getchar();
}

void CostMonthAnimal(FILE * file, char* name)
{
	system("cls");

	file = fopen(name, "rb");

	string nameAnim = "";

	cout << "Введите название животного: ";
	cin >> nameAnim;

	InfoAnimal anim;
	int costMonth = 0;

	fseek(file, 0, SEEK_END);
	long lenEmp = ftell(file);
	fseek(file, 0, SEEK_SET);

	while (ftell(file) != lenEmp)
	{
		bool coincidence = false;
		fread(&anim, sizeof(InfoAnimal), 1, file);

		if (strlen(anim.name) == nameAnim.length())
		{
			int i;
			for (i = 0; i < nameAnim.length(); i++)
			{
				if (tolower(anim.name[i]) != tolower(nameAnim[i]))
				{
					i = nameAnim.length() + 9;
				}
			}
			if (i != nameAnim.length() + 10)
				coincidence = true;
		}

		if (coincidence)
			break;
	}

	cout << "На содержание данного животного в месяц тратится: " + to_string(anim.cost * 30) << endl;

	fclose(file);

	cout << "Чтобы выйти в меню нажмите Enter...";
	cin.get();
	getchar();
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
		cout << "4 - количество животных из определенной зоны" << endl;
		cout << "5 - затраты на определнного животного в месяц" << endl;
		cout << "0 - выйти в меню" << endl;
		cout << "Введите номер задания: ";
		int numLesson;
		cin >> numLesson;

		switch (numLesson)
		{
		case 1:
			AddRecord(BinarFile, nameFile);
			break;
		case 2:
			DeleteRecord(BinarFile, nameFile);
			break;
		case 3:
			EditRecord(BinarFile, nameFile);
			break;
		case 4:
			NumbAnimal(BinarFile, nameFile);
			break;
		case 5:
			CostMonthAnimal(BinarFile, nameFile);
			break;
		case 0:
			funct = false;
			break;
		}
	}
	system("PAUSE");
}