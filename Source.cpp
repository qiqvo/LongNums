#include <iostream>
#include <fstream>
#include <string>
#include "Long.h"
#include <ctime>

#include <thread>
#include <future>

 using namespace std;
 

void print(const char* fname, const Long& num, int mod) {
	ofstream log(fname);
	log.close();
	log.open(fname, ios::app);
	Long c = null;
	switch (mod) {
	case 0:
		c = num.mul(num);
		break;
	case 1:
		c = num.karac_mul(num);
		break;
	case 2:
		c = num.toomcook_mul(num);
		break;
	case 3:
		c = num.strassen_mul(num);
		break;
	default:
		break;
	}
	log << c << "\n\n\n";
	log.close();
}

int main() {

	//Long c = "70325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458846845241607032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245870325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458846845241607032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245870325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458846845241607032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245870325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458846845241607032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245870325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836735135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324588468452416070325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324588468452416070325678965131513541321351327849311521002888629258367278493115210028886292583672784931152100288862925836713513513521610144576234925983745823915672784931152100288862925836738088862944654562135131351351352161014457625615251321351315513541321351313513513521610144576234925983745823915678088865135413213514465456561513213513151354121789651315135413213513135135135216101445762349259837458239156735131351351352161014457622151354132135131351351352161014457623492598374582391567808886294465456213535131351351352161311313513513521610144576234925983745856181219651623915678088865135413213513135135135216101445762349259837458239156780888629446545621352944654562135151354132135131351351352161014457623492598374582391567808886294465456213535413213513135135135216132138513213583135158468347452834652863474528346528634528364582362458528364582365656151321351315135459837458239172359178235162365824367583465283465836283674528367452836745283465286345283645823624582346258367529483834652836458516244616132458656515365165165165132248468452416";
	//Long a = "2784931152100288151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245884684524160703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324587032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245884684524160703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324587032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245884684524160703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324587032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493115210028886292583673808886294465456213513135135135216101445762561525132135131551354132135131351351352161014457623492598374582391567808886513541321351446545656151321351315135412178965131513541321351313513513521610144576234925983745823915673513135135135216101445762215135413213513135135135216101445762349259837458239156780888629446545621353513135135135216131131351351352161014457623492598374585618121965162391567808886513541321351313513513521610144576234925983745823915678088862944654562135294465456213515135413213513135135135216101445762349259837458239156780888629446545621353541321351313513513521613213851321358313515846834745283465286347452834652863452836458236245852836458236565615132135131513545983745823917235917823516236582436758346528346583628367452836745283674528346528634528364582362458234625836752948383465283645851624461613245884684524160703256789651315135413213513278493115210028886292583672784931152100288862925836727849311521002888629258367135135135216101445762349259837458239156727849311521002888629258367380888629446545621351313513513521610144576256152513213513155135413213513135135135216101445762349259837458239156780888651354132135144654565615132135131513541217896513151354132135131351351352161014457623492598374582391567351313513513521610144576221513541321351313513513521610144576234925983745823915678088862944654562135351313513513521613113135135135216101445762349259837458561812196516239156780888651354132135131351351352161014457623492598374582391567808886294465456213529446545621351513541321351313513513521610144576234925983745823915678088862944654562135354132135131351351352161321385132135831351584683474528346528634745283465286345283645823624585283645823656561513213513151354598374582391723591782351623658243675834652834658362836745283674528367452834652863452836458236245823462583675294838346528364585162446161324587032567896513151354132135132784931152100288862925836727849311521002888629258367278493115210028886292583671351351352161014457623492598374582391567278493";
	Long a = "34000442",	c = "6458488748418949511232228454532";
	cout << isodd(a) << "  " << iseven(a) << endl;
//	system("pause");
	//c = c * c;
	//Long a = 10915632, c = 686;
	//Long c = "-4";
	Long n = 121;
	Long b = 879190661;
//	ofstream log("log.txt");
//	log.close();
//	log.open("log.txt", ios::app);
//	log.close();
	/*
	auto t = clock();
	cout << prtest_SolovStras(99194853094755497, 16)<<endl;
	cout << clock() - t << endl << endl;
	t = clock();
	cout << prtest_Lehmann(99194853094755497, 16) << endl;
	cout << clock() - t << endl << endl;
	t = clock();
	cout << prtest_RabinMiller(99194853094755497, 16) << endl;
	cout << clock() - t << endl << endl;

 	cout << "Size of the first number is " << a.size() * 3 << endl;
	cout << "Size of the second number is " << c.size() * 3 << endl;
	t = clock();

	cout << "Time of c = c * c is ";
	t = clock();
	cout << (c.strassen_mul(c) == c.mul(c)) << endl;
	cout << clock() - t << endl;
	cout << "Time of a = c * a is ";
	t = clock();
	a = c.strassen_mul( a);
	cout << clock() - t << endl;

	cout << "Size of the first number is " << a.size() *5 << endl;
	cout << "Size of the second number is " << c.size() *5 << endl;
	/*
	cout << "Time of c / 546162 is ";
	t = clock();
	c / 546162;
	cout << clock() - t << endl;
	cout << "Time of a / 546162 is ";
	t = clock();
	a / 546162;
	cout << clock() - t << endl;
	*/
	/*
	cout << "Time of + is ";
	t = clock();
	c + (a);
	cout << clock() - t << endl;
	cout << "Time of - is ";
	t = clock();
	c - a;
	cout << clock() - t << endl;
	cout << "Time of mul is ";
	t = clock();
    c.mul(a);
	cout << clock() - t << endl;
	cout << "Time of karac is ";
	t = clock();
	a.karac_mul(c);
	cout << clock() - t << endl;
	cout << "Time of toom3 is ";
	t = clock();
	a.toomcook_mul(c);
	cout << clock() - t << endl;
	*/
	a = a * a* a* a;
	c = a.mul(c.mul(c));
	cout << "Time of karac is ";
	auto t = clock();
	cout << (a.karac_mul(c)) << endl;
	cout << clock() - t << endl;	
	cout << "Time of toom3 is ";
	 t = clock();
	cout << (a.toomcook_mul(c)) << endl;
	cout << clock() - t << endl;
    
    int l = 0;
    cin >> l;
	return 0;
}
