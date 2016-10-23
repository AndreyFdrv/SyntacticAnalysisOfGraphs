#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <cstdlib>
extern void Step1(char *text);
extern bool isTerminal(char t);
extern void Step2(char *text);
extern bool isLeftBorder(char c);
extern bool isRightBorder(char c);
extern void substr(char *str1, char *str2, int begin, int end);
extern void begin_substr(char *str1, char *str2, int end);
extern void end_substr(char *str1, char *str2, int begin);
extern void Step3(char *text);
extern bool isRuleNew(char *text, char *rule, int right_part_begin);
extern void Step4(char *text);
extern void Step5(char *text);
struct Vertex
{
	char label[50];
	bool neig[50];
};
struct triad
{
	int v1;
	int v2;
	char N;
};
void ChangeGrammarForm(char *grammar)
{
	for (int i = 0; grammar[i] != '\0'; i++)
	{
		if ((grammar[i] == '\t') || (grammar[i] == ' '))
		{
			char begin[100];
			begin_substr(begin, grammar, i - 1);
			char end[100];
			end_substr(end, grammar, i + 1);
			strcat(begin, end);
			strcpy(grammar, begin);
			i--;
		}
	}
	char cur_nonterminal = grammar[0], cur_index = 0;
	bool is_new_rule=true;
	while (is_new_rule)
	{
		for (int i = cur_index+3; grammar[i] != '\0'; i++)
		{
			if ((grammar[i] == cur_nonterminal) && (grammar[i + 1] == '-'))
			{
				int j=i+3;
				for (int j = i + 3; grammar[j] != '\n'; j++);
				char right_part[50];
				substr(right_part, grammar, i + 3, j);
				char begin[100];
				begin_substr(begin, grammar, i - 1);
				char end[100];
				end_substr(end, grammar, j + 1);
				strcat(begin, end);
				strcpy(grammar, begin);
				for (j = cur_index + 3; grammar[j] != '\n'; j++);
				begin_substr(begin, grammar, j - 1);
				strcat(begin, "|");
				strcat(begin, right_part);
				end_substr(end, grammar, j + 1);
				strcat(begin, end);
				strcpy(grammar, begin);
				i = cur_index + 3;
			}
		}
		is_new_rule = false;
		for (int k = cur_index + 3; grammar[k] != '\0'; k++)
		{
			if (grammar[k] == '\n')
			{
				cur_nonterminal = grammar[k + 1];
				cur_index = k + 1;
				is_new_rule = true;
				break;
			}
		}
	}
}
int main(int argc, char *argv[])
{
	FILE* grammar_file=fopen(argv[1],"r");
	FILE* graph_file = fopen(argv[2], "r");
	if (grammar_file == NULL)
		printf("File of grammar does not exist\n");
	else if (graph_file == NULL)
		printf("File of graph does not exist\n");
	else
	{
		char grammar[100] = "\0";
		char* str_err;
		char str[50];
		str_err = fgets(str, 50, grammar_file);
		char nonterminals[20];
		int nonterminals_index = 0;
		while (str_err != NULL)
		{
			if (str[0] != 10)
			{
				strcat(grammar, str);
				str_err = fgets(str, 50, grammar_file);
			}
			else
				str_err = fgets(str, 50, grammar_file);
		}
		ChangeGrammarForm(grammar);
		Step1(grammar);
		Step2(grammar);
		Step3(grammar);
		Step5(grammar);
		printf("The grammar after normalization:\n");
		printf("%s\n", grammar);
		nonterminals[nonterminals_index] = grammar[0];
		nonterminals_index++;
		for (int i = 0; grammar[i] != '\0'; i++)
		{
			if (grammar[i] == '\n')
			{
				nonterminals[nonterminals_index] = grammar[i+1];
				nonterminals_index++;
			}
		}
		fclose(grammar_file);
		char graph_str[300] = "\0";
		str_err = fgets(str, 50, graph_file);
		while (str_err != NULL)
		{
			if (str[0] != 10)
			{
				strcat(graph_str, str);
				str_err = fgets(str, 50, graph_file);
			}
			else
				str_err = fgets(str, 50, graph_file);
		}
		fclose(graph_file);
		Vertex graph[50];
		bool vertex_list[50];
		for (int i = 0; i < 50; i++)
			vertex_list[i] = false;
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
				graph[i].neig[j] = false;
		}
		int max_vertex=0, max_end_vertex=0;
		for (int i = 0; graph_str[i] != '\0'; i++)
		{
			if ((graph_str[i] >= '0') && (graph_str[i] <= '9'))
			{
				int j = i;
				while ((graph_str[j] >= '0') && (graph_str[j] <= '9'))
					j++;
				char number[5];
				substr(number, graph_str, i, j - 1);
				int begin_vertex = atoi(number);
				i = j;
				while (!((graph_str[i] >= '0') && (graph_str[i] <= '9')))
					i++;
				j = i;
				while ((graph_str[j] >= '0') && (graph_str[j] <= '9'))
					j++;
				substr(number, graph_str, i, j - 1);
				int end_vertex = atoi(number);
				i = j;
				while (graph_str[i] != '\"')
					i++;
				char label = graph_str[i + 1];
				graph[begin_vertex].neig[end_vertex] = true;
				graph[begin_vertex].label[end_vertex] = label;
				vertex_list[begin_vertex] = true;
				vertex_list[end_vertex] = true;
				if (begin_vertex > max_vertex)
					max_vertex = begin_vertex;
				if (end_vertex > max_end_vertex)
					max_end_vertex = end_vertex;
			}
		}
		triad r[1000];
		int r_index = 0;
		for (int i = 0; i <= max_vertex; i++)
		{
			for (int j = 0; j <= max_end_vertex; j++)
			{
				if (graph[i].neig[j] == true)
				{
					char symbol[2];
					symbol[0] = graph[i].label[j];
					symbol[1] = '\0';
					char cur_nonterminal, left_nonterminal = grammar[0];
					int nonterminal_index, left_nonoterminal_index = 0;
					for (int k = 2; grammar[k] != '\0';)
					{
						int left_border, right_border;
						if (grammar[k] == '\n')
						{
							left_nonterminal = grammar[k + 1];
							left_nonoterminal_index = k + 1;
						}
						if (isLeftBorder(grammar[k]))
						{
							left_border = k;
							k++;
							while (!isRightBorder(grammar[k]))
							{
								right_border = k + 1;
								k++;
							}
							char right_part[10];
							substr(right_part, grammar, left_border + 1, right_border - 1);
							if (!strcmp(right_part, symbol))
							{
								r[r_index].N = left_nonterminal;
								r[r_index].v1 = i;
								r[r_index].v2 = j;
								r_index++;
							}
						}
						else 
							k++;
					}
				}
			}
		}
		for (int i = 0; vertex_list[i]; i++)
		{
			if (vertex_list[i] == true)
			{
				char symbol[5];
				symbol[0] = 'e'; 
				symbol[1] = 'p';
				symbol[2] = 's';
				symbol[3] = '\0';
				char cur_nonterminal, left_nonterminal = grammar[0];
				int nonterminal_index, left_nonoterminal_index = 0;
				for (int k = 2; grammar[k] != '\0';)
				{
					int left_border, right_border;
					if (grammar[k] == '\n')
					{
						left_nonterminal = grammar[k + 1];
						left_nonoterminal_index = k + 1;
					}
					if (isLeftBorder(grammar[k]))
					{
						left_border = k;
						k++;
						while (!isRightBorder(grammar[k]))
						{
							right_border = k + 1;
							k++;
						}
						char right_part[10];
						substr(right_part, grammar, left_border + 1, right_border - 1);
						if (!strcmp(right_part, symbol))
						{
							r[r_index].N = left_nonterminal;
							r[r_index].v1 = i;
							r[r_index].v2 = i;
							r_index++;
						}
					}
					else
						k++;
				}
			}
		}
		triad r_new[1000];
		for (int i = 0; i < r_index; i++)
			r_new[i] = r[i];
		int r_new_index = r_index;
		while (r_new_index > 0)
		{
			char N1= r_new[r_new_index - 1].N;
			int n = r_new[r_new_index - 1].v1;
			int m = r_new[r_new_index - 1].v2;
			r_new_index--;
			for (int i = 0; i < r_index; i++)
			{
				if (r[i].v2==n)
				{
					char pair[5];
					pair[0] = r[i].N;
					pair[1] = N1;
					pair[2] = '\0';
					char cur_nonterminal, left_nonterminal = grammar[0];
					int nonterminal_index, left_nonoterminal_index = 0;
					for (int k = 2; grammar[k] != '\0';)
					{
						int left_border, right_border;
						if (grammar[k] == '\n')
						{
							left_nonterminal = grammar[k + 1];
							left_nonoterminal_index = k + 1;
						}
						if (isLeftBorder(grammar[k]))
						{
							left_border = k;
							k++;
							while (!isRightBorder(grammar[k]))
							{
								right_border = k + 1;
								k++;
							}
							char right_part[10];
							substr(right_part, grammar, left_border + 1, right_border - 1);
							if (!strcmp(right_part, pair))
							{
								bool is_new = true;
								for (int z = 0; z < r_index; z++)
								{
									if ((r[z].N == left_nonterminal) && (r[z].v1 == r[i].v1) && (r[z].v2 == m))
										is_new = false;
								}
								if (is_new)
								{
									r_new[r_new_index].N = left_nonterminal;
									r_new[r_new_index].v1 = r[i].v1;
									r_new[r_new_index].v2 = m;
									r_new_index++;
									r[r_index].N = left_nonterminal;
									r[r_index].v1 = r[i].v1;
									r[r_index].v2 = m;
									r_index++;
								}
							}
						}
						else
							k++;
					}
				}
			}
			for (int i = 0; i < r_index; i++)
			{
				if (r[i].v1 == m)
				{
					char pair[5];
					pair[0] = N1;
					pair[1] = r[i].N;
					pair[2] = '\0';
					char cur_nonterminal, left_nonterminal = grammar[0];
					int nonterminal_index, left_nonoterminal_index = 0;
					for (int k = 2; grammar[k] != '\0';)
					{
						int left_border, right_border;
						if (grammar[k] == '\n')
						{
							left_nonterminal = grammar[k + 1];
							left_nonoterminal_index = k + 1;
						}
						if (isLeftBorder(grammar[k]))
						{
							left_border = k;
							k++;
							while (!isRightBorder(grammar[k]))
							{
								right_border = k + 1;
								k++;
							}
							char right_part[10];
							substr(right_part, grammar, left_border + 1, right_border - 1);
							if (!strcmp(right_part, pair))
							{
								bool is_new = true;
								for (int z = 0; z < r_index; z++)
								{
									if ((r[z].N == left_nonterminal) && (r[z].v1 == n) && (r[z].v2 == r[i].v2))
										is_new = false;
								}
								if (is_new)
								{
									r_new[r_new_index].N = left_nonterminal;
									r_new[r_new_index].v1 = n;
									r_new[r_new_index].v2 = r[i].v2;
									r_new_index++;
									r[r_index].N = left_nonterminal;
									r[r_index].v1 = n;
									r[r_index].v2 = r[i].v2;
									r_index++;
								}
							}
						}
						else
							k++;
					}
				}
			}
		}
		printf("\nResult:\n");
		for (int i = 0; i < r_index; i++)
			printf("(%c, %d, %d)\n", r[i].N, r[i].v1, r[i].v2);
	}
}
