// transport_task.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

double distance(double x1, double y1, double x2, double y2)
{
	double hx, hy;
	hx = (x1 - x2)*(x1 - x2);
	hy = (y1 - y2)*(y1 - y2);
	return sqrt(hx + hy);
}

class obj_train
{
public:
	vector<unsigned int> time;
	vector<double> x;
	vector<double> y;
	string id;
	int i;
	obj_train() :x(), y(), time(), id()
	{
		i = 0;
	}
	void	push(unsigned int t, double x0, double y0)
	{
		time.push_back(t);

		x.push_back(x0);

		y.push_back(y0);

		i++;


	}

	void print()
	{
		for (int j = 0; j < i; j++)
			cout << time[j] << " " << x[j] << " " << y[j] << endl;
	}

	void write()
	{
		ofstream fout("answer.txt");
		if (fout.is_open())
			for (int j = 0; j < i; j++)
				fout << time[j] << " " << x[j] << " " << y[j] << endl;
		else
			cout << "Undefined Error!" << endl;
		fout.close();
	}

	

	unsigned int teach_obj(double dist,double tx,double ty)
	{
		unsigned int help_time;
		unsigned int med_t = 0;
		int iter = 0;
		int ind = 0;
		for (int j = 0; j < i; j++)
		{
			if ((distance(tx, ty, x[j], y[j]) < dist) && (ind == 0))
			{
				help_time = time[j];

				ind = 1;

				
			}
			if ( ( ind == 1 ) && ( ( distance(tx, ty, x[j], y[j]) > dist ) ||( j== i - 1) ) )
			{
				med_t = med_t + (time[j - 1] - help_time);
				iter++;
				help_time = 0;
				ind = 0;
			}

		}
		if (iter != 0)
			return (med_t / iter);
		else
			return 0;
	}

};

class node
{
public:

	obj_train* vol;
	node* next;
	node* previous;
	node()
	{
		vol = new obj_train();
		next = NULL;
		previous = NULL;
	}
};


class point_node
{
public:
	double x;

	double y;

	int numofpoi;

	point_node* next;

	point_node() :x(), y()
	{
		numofpoi = 0;

		next = NULL;

	}
	~point_node()
	{
		if (this->next != NULL)
			delete this->next;
	}
};


class mac_lea
{
public:
	point_node* first;

	mac_lea()
	{
		first = NULL;
	}

	void add_point(double hx, double hy, int count,int dist)
	{
		point_node* iter = first;

		point_node* prev;

		if (iter == NULL)
		{
			iter = new point_node();

			iter->x = hx;

			iter->y = hy;

			iter->numofpoi = count;

			first = iter;

			return;
		}
		else
			while (iter != NULL)
			{
			if (distance(iter->x, iter->y, hx, hy) < dist)
			{
				iter->x = (iter->x + hx) / 2.0;

				iter->y = (iter->y + hy) / 2.0;

				iter->numofpoi += count;

				return;
			}
			prev = iter;
			iter = iter->next;
			}
		iter = new point_node();

		iter->x = hx;

		iter->y = hy;

		iter->numofpoi = count;

		prev->next = iter;

	}

	void add_point(point_node* temp)
	{

		point_node* help = first;

		if (first == NULL)
		{
			first = new point_node();

			first->numofpoi = temp->numofpoi;

			first->x = temp->x;

			first->y = temp->y;

		}
		else
		{
			while (help->next != NULL)
			{
				help = help->next;

				if ((help->x == temp->x) && (help->y == temp->y) && (help->numofpoi == temp->numofpoi))
					return;
			}

			help->next = new point_node();

			help->next->numofpoi = temp->numofpoi;

			help->next->x = temp->x;

			help->next->y = temp->y;
		}
	}

	mac_lea* find_best()
	{
		int count[39];

		point_node* best[39];

		point_node* help;

		int help_c = 0;

		point_node* temp = NULL;

		int true_st = 1;

		for (int j = 0; j < 39; j++)
		{
			count[j] = 0;
			
			best[j] = NULL;
			
		}
		
		for (int j = 0; j <39; j++)
		{
			help = first;

			help_c = 0;

			temp = NULL;

			while (help != NULL)
			{
				if (j == 0)
				{
					if (help->numofpoi > help_c)
					{
						help_c = help->numofpoi;

						temp = help;
					}
				}
				else
				{
					for (int k = 0; k < j; k++)
						if (help == best[k])
							true_st = 0;
					if ((help->numofpoi > help_c) && (help->numofpoi <= count[j - 1]) && (true_st ==1  ))
					{
						help_c = help->numofpoi;

						temp = help;
					}
					true_st = 1;
				}

				help = help->next;
			}
			count[j] = help_c;

			best[j] = temp;
		}

		mac_lea* ans = new mac_lea();

		for (int j = 0; j < 39; j++)
			if (count[j] != 0)
				ans->add_point(best[j]);
			else
				break;

		return ans;


	}

	void print()
	{
		point_node* temp = first;

		while (temp != NULL)
		{
			cout << temp->x << '\t' << temp->y << '\t' << temp->numofpoi << endl;
			temp = temp->next;
		}
	}

	double find_error(double x1, double y1)
	{
		double er = 0;
		
		point_node* temp = first;

		while (temp != NULL)
		{
			if ((er == 0) || (distance(x1, y1, temp->x, temp->y) < er))
			{
				er = distance(x1, y1, temp->x, temp->y);
			}
			temp = temp->next;
		}
		return er;
	}

	void write()
	{
		point_node* iter = first;
		ofstream fout("ex_output.txt");
		if (fout.is_open())
			while (iter != NULL)
			{

			fout <<iter->x<< '\t' << iter->y <<'\t'<<iter->numofpoi<< endl;

			iter = iter->next;
			}
		else
			cout << "Undefined Error!" << endl;
		fout.close();
		
		
	}

	void read()
	{
		ifstream fin("ex_output.txt");

		int t;

		double x, y;

		point_node* help = new point_node();

		if (fin.is_open())
		{
			while (!fin.eof())
			{
				fin >> x >> y>>t;

				help->x = x;

				help->y = y;

				help->numofpoi = t;

				add_point(help);
			}
		}
	}

	void write_final_answer()
	{
		point_node* help = first;

		ofstream fout("final_answer.txt");

		while (help != NULL)
		{
			if (help->next != NULL)
				fout << help->x << '\t' << help->y << '\n';
			else
				fout << help->x << '\t' << help->y;
			help = help->next;
		}
	}

	void try_optimize(double dist)
	{ 
		point_node* now ;

		point_node* iter;

		point_node* prev;

		now = first;  

		while (now != NULL)
		{
			iter = first;

			prev = first;
			while (iter != NULL)
			{
				if ((distance(iter->x, iter->y, now->x, now->y) < dist) && (iter != now))
				{
					if (double(iter->numofpoi) / double(now->numofpoi) >= 0.4)
					{
						if (iter != first)
						{
							now->x = (now->x + iter->x) / 2;

							now->y = (now->y + iter->y) / 2;

							now->numofpoi += iter->numofpoi;
							if (iter != prev)
							{
								prev->next = iter->next;

								iter->next = NULL;
								delete iter;

								iter = prev->next;
							}
							else
							{
								prev = iter->next;
								iter->next = NULL;
								delete iter;
								iter = prev;
							}

							continue;
						}
						else
						{
							now->x = (now->x + iter->x) / 2;

							now->y = (now->y + iter->y) / 2;

							now->numofpoi += iter->numofpoi;

							prev = iter->next;

							iter->next = NULL;

							delete iter;


							iter = prev;

							first = iter;

							continue;
						}


					}
					else
					{
						if (iter != first)
						{
							prev->next = iter->next;

							iter = prev->next;
						}
						else
						{
							prev = iter->next;
							iter = prev;
							first = iter;

						}
					}
				}
				
				prev = iter;

				iter = iter->next;
			}
		
			now = now->next;

		}

	}

	mac_lea* sort_for_answer()
	{
		mac_lea* help = this->find_best();

		point_node* help_this = this->first;

		point_node* help_best = help->first;

		mac_lea* answer = new mac_lea();

		
			while (help_this != NULL)
			{
				help_best = help->first;
				while (help_best != NULL)
				{
					if ((help_this->x == help_best->x) && (help_this->y == help_best->y))
					{
						answer->add_point(help_this);
						break;
					}
					help_best = help_best->next;
				}
				help_this = help_this->next;
			}

			return answer;
		
	}

	~mac_lea()
	{
		if (this->first != NULL)
			delete this->first;
	}
};

class List
{
public:
	node* main;
	int it;
	mac_lea* solution;
	double px[3];
	double py[3];
	unsigned int ttime_g;

List()
{
	main = NULL;
	it = 0;
	px[0] = 11038.08464497;
	py[0] = 8253.17542416;

	px[1] = 283.08479678;
	py[1] = 163.45489494;

	px[2] = 3425.67079005;
	py[2] = 3469.94198377;

	solution = new mac_lea();

	ttime_g = 0;
}

void push(unsigned int t, double x0, double y0, string id0)
{
	node* iter = main;

	node* prev;

	if (iter != NULL)
	{ 
		while (iter != NULL)
		{
			if (iter->vol->id == id0)
			{
				iter->vol->push(t, x0, y0);

				return;
			}
			else
			{
				prev = iter;

				iter = iter->next;
			}
		}
			
		iter = new node();

		iter->previous = prev;

		iter->vol->id = id0;

		prev->next = iter;

		iter->vol->push(t, x0, y0);

		it++;

		return;

	}
	else
	{
		main = new node();

		main->vol->id = id0;

		main->vol->push(t, x0, y0);

		it++;
	}
}

void print(string id0)
{
	node* iter = main;

	while (iter != NULL)
	{
		if (iter->vol->id == id0)
		{
			iter->vol->print();
			return;
		}
		iter = iter->next;
	}
	cout << "Error: can't find id " << id0 << endl;
}

void write(string id0)
{
	node* iter = main;

	while (iter != NULL)
	{
		if (iter->vol->id == id0)
		{
			iter->vol->write();
			return;
		}
		iter = iter->next;
	}
	cout << "Error: can't find id " << id0 << endl;
}

unsigned int teach_system(double dist,double tx,double ty)
{
	unsigned int medium = 0;

	node* iter = main;

	int num = 0;

	unsigned int help;

	while (iter != NULL)
	{
		help = iter->vol->teach_obj(dist, tx, ty);

		if (help != 0)
		{
			medium = medium + help;
			num++;
		}
		iter = iter->next;
	}

	if (num != 0)
		return (medium / num);
	else
		return 0;
}

void find_solution(int dist,double mn_div,double mn_umn)
{
	delete solution;

	solution = new mac_lea();

	node* temp = main;
	double tx, ty, mx, my;
	unsigned int tt;
	unsigned int ttime = 0;
	int numofpoint = 0;
	

	ttime = ttime_g / mn_div;

	while (temp != NULL)
	{
		tx = temp->vol->x[0]; ty = temp->vol->y[0];
		tt = temp->vol->time[0]; numofpoint = 1;
		mx = temp->vol->x[0]; my = temp->vol->y[0];
		for (int j = 1; j < temp->vol->i; j++)
		{
			if ((distance(tx, ty, temp->vol->x[j], temp->vol->y[j]) < mn_umn*dist) && ((temp->vol->time[j] - tt) <= ttime))
			{
				numofpoint++;
				mx = (mx + temp->vol->x[j]) / 2.0;

				my = (my + temp->vol->y[j]) / 2.0;
				
			}
			else
			{
				if (numofpoint > 1)
				{
					solution->add_point(mx, my, numofpoint, mn_umn*dist);
					numofpoint = 1;
					mx = temp->vol->x[j];
					my = temp->vol->y[j];
					tx = temp->vol->x[j];
					ty = temp->vol->y[j];
					tt = temp->vol->time[j];
				}
				else
				{
					numofpoint = 1;
					mx = temp->vol->x[j];
					my = temp->vol->y[j];
					tx = temp->vol->x[j];
					ty = temp->vol->y[j];
					tt = temp->vol->time[j];
				}
			}
		}

		temp = temp->next;
	}

}

void evaluate_ttime(int dist)
{
	ttime_g = 0;
	for (int i = 0; i < 3; i++)
		ttime_g += teach_system(dist, px[i], py[i]);
}

mac_lea* find_best_solution(double er)
{
	mac_lea* temp;
	mac_lea* best = NULL;
	double best_er = 0;
	double help_er = 0; 
	int ind = 1;
	int terator = 0;
	int ui = 0;
	double u, d; int di = 0; double b_u, b_d; int b_di;
	u = 0; b_u = 0;
	d = 0; b_d = 0;
	b_di = 0;
	for (int dist = 121; dist < 122; dist++)
	{
		evaluate_ttime(dist);
		
		for (double div = 2.0; div <= 3; div += 0.05)
		{
			for (double umn = 1.5; umn <= 2.0; umn += 0.05)
			{
				ind = 1;
				terator++;
				help_er = 0;
				find_solution(dist, div, umn);

				temp = solution->find_best();

				for (int j = 0; j < 3; j++)
					{
						double my_1 = temp->find_error(px[j], py[j]);
						if (my_1 > er)
							{
								ind = 0;

							}
						if (my_1 > help_er)
							help_er = my_1;
					}


				if (ind == 1)
				{
					u = umn;
					temp->write();
					break;
				}

				if ((help_er < best_er) || (best_er == 0))
				{
					if (best!=NULL)
						delete best;
					best = temp;
					b_u = umn;
					b_d = div;
					b_di = dist;
					best_er = help_er;
				}
				else
				{
					delete temp;
				}

				
			}
			if (ind == 1)
			{
				d = div;
				break;
			}
		}
		if (ind == 1)
		{
			di = dist;
			break;
		}
		if ((terator / 1000) > ui)
		{
			cout << "iteration : " << terator << endl;
			ui++;
		}
	}

	

	if (ind == 1)
	{
		cout << "u = " << u << '\t' << "d = " << d << '\t' << "distance = " << di << endl;
		return temp;
	}
	else
	{
		cout << "b_u = " << b_u << '\t' << "b_d = " << b_d << '\t' << "b_distance = " << b_di << '\t' << "best_er = " << best_er << endl;
		return best;
	}
	
}
};


mac_lea* sort_points(mac_lea* ar_p, obj_train* ex_w, double dist)
{
	double px[2], py[2];

	px[0] = 11038.08464497;
	py[0] = 8253.17542416;

	px[1] = 283.08479678;
	py[1] = 163.45489494;

	int iter = 0;

	mac_lea* answer = new mac_lea();

	while (distance(px[0], py[0], ex_w->x[iter], ex_w->y[iter]) > dist)
		iter++;
	point_node* help;

	while (iter < ex_w->i)
	{
		help = ar_p->first;

		while (help != NULL)
		{
			if (distance(help->x, help->y, ex_w->x[iter], ex_w->y[iter]) < dist)
			{
				answer->add_point(help);

			}

			help = help->next;
		}

		iter++;
	}
	return answer;
}


int main()
{
	/*ifstream fin("C:\\train.txt");

	unsigned int t;

	double x, y;

	char id[7];

	string my;

	List work;

	

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> t >> x >> y;
			fin.getline(id, 7);
			
			if (id[0] != '\t')
				id[0] = '\t';

				my = id;

				work.push(t, x, y, my);
		}
	}

	int q;
	cout << "Do you want find solution? 1 - yes"<< endl;
	cin >> q;
	mac_lea* good;
	if (q == 1)
	{
		good = work.find_best_solution(26.0);

		work.solution->write();

		good->print();
	}
*/



	/*
	mac_lea* my1 = new mac_lea();

	my1->read();

	//cout << "read complete" << endl;

	my1->try_optimize(90.0);

	ifstream fin("C:\\example_way.txt");

	unsigned int t;

	double x, y;

	obj_train* obj_read = new obj_train();

	obj_read->id = "help";

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> t >> x >> y;
			
			obj_read->push(t, x, y);
			
		}
	}

	mac_lea* obj1, *obj2;

	obj1 = sort_points(my1, obj_read, 120);

	obj2 = obj1->sort_for_answer();

	obj2->write();

	obj2->print();
	*/
	
	mac_lea* ans = new mac_lea();

	ans->read();

	ans->print();

	ans->write_final_answer();
	
	

	system("pause");
	return 0;
}

