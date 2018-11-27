#include<iostream>
#include<math.h>
#include<iomanip>
#include<fstream>
using namespace std;
float matrixX[100][100];
float arrayGiaTriTB[100];
float matrixChuanHoaX[100][100];
float matrixSimilarityUser[100][100];
float matrixSimilarY[100][100];
float matrixY[100][100];
ifstream fileX;
ofstream fileY("TextY.txt");
int row, col;



//Nhap vao arrayX
void input(float matrixX[100][100]) {
	fileX.open("TextX.txt");
	fileX >> row;
	fileX >> col;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			fileX >> matrixX[i][j];
		}
	}
	fileX.close();
}

//Xuat ra array
void outputConsole(int row, int col, float matrix[100][100]) {

	cout << "      ";
	for (int i = 0; i < col; i++) {
		cout << setw(5) << "u" << i;
	}
	cout << endl << "   " << "---------------------------------------" << endl << endl;
	for (int i = 0; i < row; i++) {
		cout << setw(2) << "i" << i << " |  ";
		for (int j = 0; j < col; j++) {
			cout << setw(6) << matrix[i][j];
		}
		cout << endl;
	}
}

//Tim mang cac gia tri trung binh cua tung user
void averageOfCol(int row, int col) {
	int count = 0;
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			if (matrixX[j][i] >= 0 && matrixX[j][i] <= 5) {
				arrayGiaTriTB[i] += matrixX[j][i];
				count++;
			}
		}
		arrayGiaTriTB[i] = floorf(arrayGiaTriTB[i] / count * 100) / 100;
		count = 0;
	}
	cout << "Mang cac gia tri trung binh cua user: " << endl;
	cout << "      ";
	for (int i = 0; i < col; i++) {
		cout << setw(10) << "utb" << i << ": " << arrayGiaTriTB[i];
	}
	cout << endl;
}

//Ma tran chuan hoa X. (Normalized ultility maxtrix X)
void matrixNormalized(int row, int col) {
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			if (matrixX[j][i] >= 0 && matrixX[j][i] <= 5) {
				matrixChuanHoaX[j][i] = matrixX[j][i] - arrayGiaTriTB[i];
			}
			else {
				matrixChuanHoaX[j][i] = 0;
			}

		}
	}
	cout << endl;
	cout << "Ma Tran X da chuan hoa (Normalized ultility maxtrix X) : " << endl;
	outputConsole(row, col, matrixChuanHoaX);
}

//Ham Cosine Similarity: cos(u1,u2) = u1u2/(sqrt(u1^2)*(sqrt(u2^2)
float cos2Vector(int row, float userX[], float userY[]) {
	float sum = 0;
	float userXmu2 = 0;
	float userYmu2 = 0;

	for (int i = 0; i < row; i++) {
		sum += userX[i] * userY[i];
		userXmu2 += userX[i] * userX[i];
		userYmu2 += userY[i] * userY[i];

	}

	return floorf(sum / (sqrt(userXmu2)*sqrt(userYmu2)) * 100) / 100;
}

//Ma tran User similarity matrix S
void UserSmilarityMatrix(int col) {
	float userX[100], userY[100];

	for (int i = 0; i < col; i++) {
		for (int j = 0; j < col; j++) {
			for (int k = 0; k < col; k++) {
				userX[k] = matrixChuanHoaX[k][j];
				userY[k] = matrixChuanHoaX[k][i];
			}
			matrixSimilarityUser[i][j] = cos2Vector(col, userX, userY);
		}
	}
	cout << endl;
	cout << "Ma tran User similarity " << endl;
	outputConsole(col, col, matrixSimilarityUser);
}

void SmilarityMatrixY(int row, int col)
{
	float max1 = -999;
	float max2 = -999;
	int userMax1;
	int userMax2;
	//Cho ma tran Y ban dau bang ma tran chuan hoa X
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrixSimilarY[i][j] = matrixChuanHoaX[i][j];
		}
	}
	//Tinh toan cac rating con thieu
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (matrixChuanHoaX[i][j] == 0)
			{
				for (int k = 0; k < col; k++)
				{
					if (matrixChuanHoaX[i][k] != 0 && matrixSimilarityUser[k][j] > max1)
					{
						max1 = matrixSimilarityUser[k][j];
						userMax1 = k;
					}
				}
				for (int k = 0; k < col; k++)
				{
					if (matrixSimilarityUser[k][j] < max1 && matrixSimilarityUser[k][j] > max2 && matrixSimilarityUser[k][j] != 1 && matrixChuanHoaX[i][k] != 0)
					{
						max2 = matrixSimilarityUser[k][j];
						userMax2 = k;
					}
				}
				cout << "Normalized Rating: " << endl;
				cout << setw(7) << "max1 = " << max1;
				cout << setw(10) << "max2 = " << max2;
				cout << setw(20) << "userMax1 = " << userMax1;
				cout << setw(14) << "userMax2 = " << userMax2 << endl;
				matrixSimilarY[i][j] = floorf((max1*matrixChuanHoaX[i][userMax1] + max2 * matrixChuanHoaX[i][userMax2]) / (abs(max1) + abs(max2)) * 100) / 100;
				max1 = -999;
				max2 = -999;

			}
		}
	}
	cout << endl;
	cout << "Ma tran du doan cac ratings con thieu  (normalized) :" << endl;
	outputConsole(row, col, matrixSimilarY);
	cout << endl;
}
void fullY(int row, int col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrixY[i][j] = matrixSimilarY[i][j] + arrayGiaTriTB[j];
		}
	}
	cout << "MA TRAN DANH GIA Y DAY DU: " << endl;
	outputConsole(row, col, matrixY);
}

void writeFile(float matrix[100][100])
{

	fileY << endl << "      ";
	for (int i = 0; i < col; i++) {
		fileY << setw(5) << "u" << i;
	}
	fileY << endl << "   " << "---------------------------------------" << endl << endl;
	for (int i = 0; i < row; i++)
	{
		fileY << setw(2) << "i" << i << " |  ";
		for (int j = 0; j < col; j++)
		{
			fileY << setw(6) << matrix[i][j];
		}
		fileY << endl;
	}

}
void outputFileY()
{
	fileY << "IN RA TEP Y !!!" << endl << endl;
	fileY << "MA TRAN X DANH GIA X (DANH GIA RATING CHI TU 0 DEN 5): " << endl;
	writeFile(matrixX); fileY << endl;

	fileY << "Mang cac gia tri trung binh cua user: " << endl;
	for (int i = 0; i < col; i++) {
		fileY << setw(10) << "utb" << i << ": " << arrayGiaTriTB[i];
	}
	fileY << endl << endl;
	fileY << "Ma Tran X da chuan hoa (Normalized ultility maxtrix X) : " << endl;
	writeFile(matrixChuanHoaX); fileY << endl;
	fileY << "Ma tran User similarity " << endl;
	writeFile(matrixSimilarityUser); fileY << endl;
	fileY << "Ma tran Y du doan cac ratings con thieu (normalized) :" << endl;
	writeFile(matrixSimilarY); fileY << endl;
	fileY << "Ma tran danh gia Y day du : " << endl;
	writeFile(matrixY); fileY << endl;
	fileY.close();
}

int main() {


	cout << "Nhap Ma Tran X tu file TextX: " << endl;
	input(matrixX);
	cout << "Ma Tran X can danh gia: " << endl;
	outputConsole(row, col, matrixX);
	cout << endl << endl;
	averageOfCol(row, col);
	matrixNormalized(row, col);
	UserSmilarityMatrix(col);
	SmilarityMatrixY(row, col);
	fullY(row, col);
	outputFileY();
	system("pause");
	return 0;
}

