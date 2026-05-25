#include <fstream>
#include <iostream>

void MainMenu();
void AcquisitionConsole();
void MembershipConsole();
void BookIssueConsole();
void BookReturnConsole();
void ManagementConsole();

// helper functions
void DisplayFileData(char*);
bool EqualStrings(char*, char*);
bool CheckMemberExistence(char*);
bool CheckAcqExistence(char*, char*, char&);
int StringLength(char*);
void UpdateAquisitionLendStatus(char*); //updates lend to 1 or 0
void AddRecordBookIssueReturn(char*, char*, char*, char*, char*, const char*);

int main()
{
	MainMenu();
	return 0;
}

void AddRecordBookIssueReturn(char* MembNO, char* Acq, char* AccNO, char* date1, char* date2, const char* fileName)
{
	std::ofstream writeFile;
	writeFile.open(fileName, std::ios::app);
	if (!writeFile.is_open())
	{
		std::cout << "File not found!!!" << std::endl;
	}
	else
	{
		writeFile << MembNO << ',' << Acq << ',' << AccNO << ',' << date1 << ',' << date2 << std::endl;
		writeFile.close();
	}
}

void UpdateAquisitionLendStatus(char* Acq)
{
	char** fileData = nullptr;
	char fileHeaders1[256];
	char fileHeaders2[256];

	fileData = new char*[10000];
	for (int i = 0; i < 10000; i++)
	{
		fileData[i] = new char[256];
	}

	std::ifstream readFile;
	readFile.open("acquisition.txt");
	if (!readFile.is_open())
	{
		std::cout << "File not Found!!!" << std::endl;
	}
	else
	{
		int rowCount = 0;
		readFile.getline(fileHeaders1, 255);
		readFile.getline(fileHeaders2, 255);
		while (!readFile.eof())
		{
			readFile.getline(fileData[rowCount], 255); // reading complete file
			rowCount++;							// counting rows
		}
		readFile.close();

		// got all the data in filedata[][]
		char temp[20];
		int tempCol = 0;
		int fileDataRow = 0;
		fileDataRow = 0;

		while (fileDataRow < rowCount)
		{
			tempCol = 0;
			while (fileData[fileDataRow][tempCol] != ',')
			{
				temp[tempCol] = fileData[fileDataRow][tempCol];
				tempCol++;
			}
			temp[tempCol] = '\0';
			if (EqualStrings(temp, Acq))
			{
				char status = fileData[fileDataRow][StringLength(fileData[fileDataRow]) - 1];
				if (status == '0') // convert to '1'
				{
					fileData[fileDataRow][StringLength(fileData[fileDataRow]) - 1] = '1';
				}
				else // status == '1'  convert to '0'
				{
					fileData[fileDataRow][StringLength(fileData[fileDataRow]) - 1] = '0';
				}
				break;
			}
			fileDataRow++;
		}

		std::ofstream writeFile;
		writeFile.open("acquisition.txt");
		if (!writeFile.is_open())
		{
			std::cout << "File not Found!!!" << std::endl;
		}
		else
		{
			fileDataRow = 0;
			writeFile << fileHeaders1 << std::endl;
			writeFile << fileHeaders2 << std::endl;
			while (fileDataRow < rowCount)
			{
				writeFile << fileData[fileDataRow] << std::endl;
				fileDataRow++;
			}
			writeFile.close();
		}
	}
	for (int i = 0; i < 10000; i++)
	{
		delete[] fileData[i];
	}
	delete[] fileData;
}

int StringLength(char* inputString)
{
	int strLen = 0;
	while (inputString[strLen] != '\0')
	{
		strLen++;
	}
	return strLen;
}

bool CheckAcqExistence(char* Acq, char* AccNo, char& bookAvailability)
{
	bool returnValue = false;
	std::ifstream readFile;
	readFile.open("acquisition.txt");
	if (!readFile.is_open())
	{
		std::cout << "File not found!!!" << std::endl;
	}
	else
	{
		char temp[256];
		readFile.getline(temp, 255);
		readFile.getline(temp, 255);
		while (!readFile.eof())
		{
			readFile.getline(temp, 255, ',');
			if (EqualStrings(temp, Acq))
			{
				readFile.getline(AccNo, 255, ',');
				readFile.getline(temp, 255);
				bookAvailability = temp[StringLength(temp) - 1];
				returnValue = true;
			}
			else
			{
				readFile.getline(temp, 255);
			}
		}
		readFile.close();
	}
	return returnValue;
}

bool CheckMemberExistence(char* MembNo)
{
	std::ifstream readFile;
	readFile.open("membership maintanence.txt");
	if (!readFile.is_open())
	{
		std::cout << "File not found!!!" << std::endl;
	}
	else
	{
		char temp[256];
		readFile.getline(temp, 255);
		readFile.getline(temp, 255);
		while (!readFile.eof())
		{
			readFile.getline(temp, 255, ',');
			if (EqualStrings(temp, MembNo))
			{
				return true;
			}
			readFile.getline(temp, 255);
		}
		readFile.close();
	}
	return false;
}

bool EqualStrings(char* string1, char* string2)
{
	int string1Len = StringLength(string1);
	int string2Len = StringLength(string2);

	if (string1Len == string2Len)
	{
		bool same = true;
		for (int i = 0; i < string2Len; i++)
		{
			if (string1[i] != string2[i])
			{
				same = false;
				break;
			}
		}
		return same;
	}

	return false;
}

void DisplayFileData(char* fileName)
{
	std::ifstream readfile;
	readfile.open(fileName);
	if (!readfile.is_open())
	{
		std::cout << "File not found!!!" << std::endl;
	}
	else
	{
		char temp[256];
		while (!readfile.eof())
		{
			for (int i = 0; i < 8; i++)
			{
				readfile.getline(temp, 255, ',');
				std::cout << temp << "\t";
			}
			readfile.getline(temp, 255);
			std::cout << temp << "\t";
			std::cout << std::endl;
		}
		readfile.close();
	}
}

void BookReturnConsole()
{
	char MembNO[20], Acq[20], AccNO[20], BookReturnedOn[20], DecidedDateOfReturn[20];

	bool firstEntry = true;
	while (true)
	{
		std::cout << "Enter Memb.NO: ";
		if (firstEntry)
		{
			std::cin.ignore();
			firstEntry = false;
		}
		std::cin.getline(MembNO, 19);
		if (CheckMemberExistence(MembNO))
		{
			break;
		}
		std::cout << "Memb.No does not exist!!!" << std::endl;
		std::cout << "Try Again..." << std::endl;
	}

	char BookAvailability = '\0', temp = '\0';
	while (true)
	{
		std::cout << "Enter Book Acq: ";
		std::cin.getline(Acq, 19);
		if (CheckAcqExistence(Acq, AccNO, BookAvailability))
		{
			break;
		}
		std::cout << "Book Acq does not exist!!!" << std::endl;
		std::cout << "Try Again..." << std::endl;
	}
	if (BookAvailability == '1') // this means book should be returned
	{
		std::cout << "Enter Book returned on Date: ";
		std::cin.getline(BookReturnedOn, 19);
		std::cout << "Enter Book decided Date of return: ";
		std::cin.getline(DecidedDateOfReturn, 19);
		UpdateAquisitionLendStatus(Acq);
		AddRecordBookIssueReturn(MembNO, Acq, AccNO, BookReturnedOn, DecidedDateOfReturn, "bookreturn.txt");
		std::cout << "Checking for fine..." << std::endl;
		if (!EqualStrings(BookReturnedOn, DecidedDateOfReturn))
		{
			std::cout << "Apply Fine for late return!!!" << std::endl;
			int FineAmount = 0;
			while (true)
			{
				std::cout << "Enter Fine Amount:";
				std::cin >> FineAmount;
				if (FineAmount > 0)
				{
					break;
				}
				std::cout << "Invalid Fine Amount!!!" << std::endl;
				std::cout << "Try Again..." << std::endl;
			}
			std::ofstream writeFile;
			writeFile.open("fineregister.txt", std::ios::app);
			if (!writeFile.is_open())
			{
				std::cout << "File not found!!!" << std::endl;
			}
			else
			{
				writeFile << MembNO << ',' << Acq << ',' << AccNO << ',' << FineAmount << std::endl;
				writeFile.close();
			}
		}
		else
		{
			std::cout << "No Fine!!!" << std::endl;
		}
	}
	else
	{
		std::cout << "Book Already Returned..." << std::endl;
	}
	std::cout << "Enter any key to continue..." << std::endl;
	std::cin >> temp;
}

void ManagementConsole()
{
	while (true)
	{
		char ACQUISITION[] = "acquisition.txt";
		char MembershipMaintanence[] = "membership maintanence.txt";
		char BOOKISSUE[] = "bookissue.txt";
		char BOOKRETURN[] = "bookreturn.txt";
		char FINEREGISTER[] = "fineregister.txt";
		system("CLS");
		std::cout << "Management Queries" << std::endl;
		std::cout << "1) Acquistion Register" << std::endl;
		std::cout << "2) Membership Register" << std::endl;
		std::cout << "3) Issue Register" << std::endl;
		std::cout << "4) Return Register" << std::endl;
		std::cout << "5) Fine Register" << std::endl;
		std::cout << "0) Return" << std::endl;
		int yourchoice;
		while (true)
		{
			std::cout << "Enter your choice" << std::endl;
			std::cin >> yourchoice;
			if (yourchoice >= 0 && yourchoice <= 5)
			{
				break;
			}
			std::cout << "Invalid input!!!" << std::endl;
			std::cout << "Try Again!!!" << std::endl;
		}
		if (yourchoice == 1)
		{
			DisplayFileData(ACQUISITION);
		}
		else if (yourchoice == 2)
		{
			DisplayFileData(MembershipMaintanence);
		}
		else if (yourchoice == 3)
		{
			DisplayFileData(BOOKISSUE);
		}
		else if (yourchoice == 4)
		{
			DisplayFileData(BOOKRETURN);
		}
		else if (yourchoice == 5)
		{
			DisplayFileData(FINEREGISTER);
		}
		else if (yourchoice == 0)
		{
			break;
		}
		char temp;
		std::cout << "Enter any key to continue..." << std::endl;
		std::cin >> temp;
	}
}

void BookIssueConsole()
{
	char MembNO[20], Acq[20], AccNO[20], BookIssuedOn[20], DateOfReturn[20];

	bool firstEntry = true;
	while (true)
	{
		std::cout << "Enter Memb.NO: ";
		if (firstEntry)
		{
			std::cin.ignore();
			firstEntry = false;
		}
		std::cin.getline(MembNO, 19);
		if (CheckMemberExistence(MembNO))
		{
			break;
		}
		std::cout << "Memb.No does not exist!!!" << std::endl;
		std::cout << "Try Again..." << std::endl;
	}

	char BookAvailability = '\0', temp = '\0';
	while (true)
	{
		std::cout << "Enter Book Acq: ";
		std::cin.getline(Acq, 19);
		if (CheckAcqExistence(Acq, AccNO, BookAvailability))
		{
			break;
		}
		std::cout << "Book Acq does not exist!!!" << std::endl;
		std::cout << "Try Again..." << std::endl;
	}
	if (BookAvailability == '1') // this means book not available to lend
	{
		std::cout << "Sorry book not available..." << std::endl;
	}
	else // BookAvailability == '0' this means book available to lend
	{
		std::cout << "Enter Book Issued Date: ";
		std::cin.getline(BookIssuedOn, 19);
		std::cout << "Enter Book Return Date: ";
		std::cin.getline(DateOfReturn, 19);
		UpdateAquisitionLendStatus(Acq);
		AddRecordBookIssueReturn(MembNO, Acq, AccNO, BookIssuedOn, DateOfReturn, "bookissue.txt");
	}
	std::cout << "Enter any key to continue..." << std::endl;
	std::cin >> temp;
}

void MembershipConsole()
{
	std::cout << "Under development" << std::endl;
	char temp;
	std::cout << "Enter any key to continue..." << std::endl;
	std::cin >> temp;
}

void AcquisitionConsole()
{
	std::cout << "Under development" << std::endl;
	char temp;
	std::cout << "Enter any key to continue..." << std::endl;
	std::cin >> temp;
}

void MainMenu()
{
	while (true)
	{
		system("CLS");
		std::cout << "Library Management System" << std::endl;
		std::cout << "0) Exit" << std::endl;
		std::cout << "1) Acquistion Section" << std::endl;
		std::cout << "2) Membership Section" << std::endl;
		std::cout << "3) Book Issue" << std::endl;
		std::cout << "4) Book Return" << std::endl;
		std::cout << "5) Management Queries" << std::endl; // Completed
		int userinput;
		while (true)
		{
			std::cout << "Enter number of the designed Operations" << std::endl;
			std::cin >> userinput;
			if (userinput >= 0 && userinput <= 5)
			{
				break;
			}
			std::cout << "Invalid input!!!" << std::endl;
			std::cout << "Try Again!!!" << std::endl;
		}
		if (userinput == 0)
		{
			break;
		}
		else if (userinput == 1)
		{
		}
		else if (userinput == 2)
		{
			MembershipConsole();
		}
		else if (userinput == 3)
		{
			BookIssueConsole();
		}
		else if (userinput == 4)
		{
			BookReturnConsole();
		}
		else /*if (userinput == 5)*/
		{
			ManagementConsole();
		}
	}
}