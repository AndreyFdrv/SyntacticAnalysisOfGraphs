#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
void Step1(char *text)
{
	char nonterminal = text[0];
	bool IsRightStartNonterminal = false;
	for (int i = 3; text[i] != '\n'; i++)
	{
		if (text[i] == nonterminal)
			IsRightStartNonterminal = true;
	}
	if (!IsRightStartNonterminal)
		return;
	char StartNonterminal[100] = "S->";
	char oldStartNonterminal[2];
	oldStartNonterminal[0] = 'A';
	oldStartNonterminal[1] = '\0';
	strcat(StartNonterminal, oldStartNonterminal);
	strcat(StartNonterminal, "\n");
	for (int i = 0; (text[i] != '\0'); i++)
	{
		if (text[i] == nonterminal)
			text[i] = 'A';
	}
	strcat(StartNonterminal, text);
	strcpy(text, StartNonterminal);
	text[strlen(text) - 1] = '\0';
}
bool isTerminal(char t)
{
	if ((t >= 'a') && (t <= 'z'))
		return true;
	if ((t == '(') || (t == ')'))
		return true;
	if ((t == '+') || (t == '*'))
		return true;
	return false;
}
void Step2(char *text)
{
	int i;
	for (i = 0; (text[i] != '\n') && (text[i] != '\0'); i++);
	char next_letter = 'B';
	if (text[i] != '\0')
	{
		i++;
		int start = i + 3;
		for (i = start; (text[i] != '\n') && (text[i] != '\0'); i++);//go to third line
		while (text[i] != '\0')
		{
			i++;
			if (text[i] != next_letter)
			{
				char nonterminal = text[i];
				for (int j = start; text[j] != '\0'; j++)
					if (text[j] == nonterminal)
						text[j] = next_letter;
			}
			for (; (text[i] != '\n') && (text[i] != '\0'); i++);
			next_letter++;
		}
	}
	int start = 3;
	while (text[start] != '\0')
	{
		for (i = start; text[i] != '\0'; i++)
		{
			if ((text[i] == 'e') && (text[i + 1] == 'p') && (text[i + 2] == 's'))
			{
				i = i + 3;
				continue;
			}
			if (isTerminal(text[i]))
				break;
		}
		start = i + 1;
		char terminal = text[i];
		if (!(((text[i - 1] == '>') || (text[i - 1] == '|')) && ((text[i + 1] == '\n') || (text[i + 1] == '\0') || (text[i + 1] == '|'))))
		{
			for (; text[i] != '\0'; i++)
			{
				if (text[i] == terminal)
					text[i] = next_letter;
			}
			char new_rule[20];
			new_rule[0] = next_letter;
			new_rule[1] = '\0';
			next_letter++;
			strcat(new_rule, "->");
			new_rule[3] = terminal;
			new_rule[4] = '\0';
			strcat(text, "\n");
			strcat(text, new_rule);
		}
	}
}
bool isLeftBorder(char c)
{
	if ((c == '>') || (c == '|'))
		return true;
	return false;
}
bool isRightBorder(char c)
{
	if ((c == '|') || (c == '\n') || (c == '\0'))
		return true;
	return false;
}
void substr(char *str1, char *str2, int begin, int end)
{
	int i;
	for (i = begin; i <= end; i++)
	{
		str1[i - begin] = str2[i];
	}
	str1[i - begin] = '\0';
}
void begin_substr(char *str1, char *str2, int end)
{
	int i;
	for (i = 0; i <= end; i++)
	{
		str1[i] = str2[i];
	}
	str1[i] = '\0';
}
void end_substr(char *str1, char *str2, int begin)
{
	int i;
	for (i = begin; str2[i] != '\0'; i++)
	{
		str1[i - begin] = str2[i];
	}
	str1[i - begin] = '\0';
}
void Step3(char *text)
{
	char next_letter = 'A' - 1;
	for (int i = 3; text[i] != '\0'; i++)
	{
		if ((text[i] > next_letter) && (text[i] <= 'Z') && (text[i] != 'S'))
			next_letter = text[i];
	}
	next_letter++;//íàøëè next_letter
	int nonterminal_count = 0;
	for (int i = 3; text[i] != '\0';)
	{
		int left_border, right_border;
		if (isLeftBorder(text[i]))
		{
			left_border = i;
			i++;
			while (!isRightBorder(text[i]))
			{
				right_border = i + 1;
				if ((text[i] >= 'A') && (text[i] <= 'Z'))
					nonterminal_count++;
				i++;
			}
		}
		else
			i++;
		if (nonterminal_count > 2)
		{
			char begin_str[100];
			begin_substr(begin_str, text, left_border + 1);
			char middle_str[2];
			middle_str[0] = next_letter;
			middle_str[1] = '\0';
			char end_str[100];
			end_substr(end_str, text, right_border);
			char new_rule[20];
			new_rule[0] = '\n';
			new_rule[1] = next_letter;
			new_rule[2] = '\0';
			next_letter++;
			strcat(new_rule, "->");
			new_rule[4] = '\0';
			char new_rule_right_part[10];
			substr(new_rule_right_part, text, left_border + 2, right_border - 1);
			strcat(new_rule, new_rule_right_part);
			strcat(begin_str, middle_str);
			strcat(begin_str, end_str);
			strcat(begin_str, new_rule);
			strcpy(text, begin_str);
		}
		nonterminal_count = 0;
	}
}
bool isRuleNew(char *text, char *rule, int right_part_begin)
{
	for (int i = right_part_begin; (text[i] != '\n') && (text[i] != '\0');)
	{
		int left_border, right_border;
		if (isLeftBorder(text[i - 1]))
		{
			left_border = i - 1;
			while (!isRightBorder(text[i]))
			{
				right_border = i + 1;
				i++;
			}
		}
		else
		{
			i++;
			continue;
		}
		char cur_rule[20];
		substr(cur_rule, text, left_border + 1, right_border - 1);
		if (!strcmp(rule, cur_rule))
			return false;
	}
	return true;
}
void Step4(char *text)
{
	char cur_nonterminal = text[0];
	bool is_eps = false;
	int right_part_begin;
	int eps_i;
	while (true)
	{
		bool is_start = true;
		bool is_eps = false;
		for (int i = 3; text[i] != '\0'; i++)
		{
			if (text[i] == '\n')
			{
				is_start = false;
				cur_nonterminal = text[i + 1];
				right_part_begin = i + 4;
			}
			if ((text[i] == 'e') && (text[i + 1] == 'p') && (text[i + 2] == 's') && (!is_start))
			{
				is_eps = true;
				eps_i = i;
				break;
			}
		}
		if (!is_eps)
			return;
		char begin_str[100];
		begin_substr(begin_str, text, eps_i - 2);
		if (text[eps_i - 2] == '-')
			strcat(begin_str, ">");
		char end_str[100];
		end_substr(end_str, text, eps_i + 3);
		strcat(begin_str, end_str);
		strcpy(text, begin_str);
		for (int i = 3; text[i] != '\0';)
		{
			int left_border, right_border;
			bool is_cur_nonterminal = false;
			if (isLeftBorder(text[i - 1]))
			{
				left_border = i - 1;
				while (!isRightBorder(text[i]))
				{
					right_border = i + 1;
					if (text[i] == cur_nonterminal)
						is_cur_nonterminal = true;
					i++;
				}
				if (text[i] == '\n')
					i = i + 4;
			}
			else
				i++;
			if (is_cur_nonterminal)
			{
				for (int j = left_border + 1; j < right_border; j++)
				{
					if (text[j] == cur_nonterminal)
					{
						char new_rule[20];
						if (j == left_border + 1)
							new_rule[0] = '\0';
						else
							substr(new_rule, text, left_border + 1, j - 1);
						char right_new_rule[20];
						if (j == right_border - 1)
							right_new_rule[0] = '\0';
						else
							substr(right_new_rule, text, j + 1, right_border - 1);
						strcat(new_rule, right_new_rule);
						if (new_rule[0] == '\0')
						{
							strcpy(new_rule, "eps");
						}
						bool similar_left_and_right = false;
						if ((new_rule[0] == cur_nonterminal) && (new_rule[1] == '\0'))
							similar_left_and_right = true;
						if ((isRuleNew(text, new_rule, right_part_begin)) && (!similar_left_and_right))
						{
							int k;
							for (k = left_border; (text[k] != '\n') && (text[k] != '\0'); k++);
							char begin_str[100];
							begin_substr(begin_str, text, k - 1);
							char middle_str[20];
							middle_str[0] = '|';
							middle_str[1] = '\0';
							strcat(middle_str, new_rule);
							if (i >= k)
								i = i + strlen(middle_str);
							char end_str[100];
							end_substr(end_str, text, k);
							strcat(begin_str, middle_str);
							strcat(begin_str, end_str);
							strcpy(text, begin_str);
							strcpy(begin_str, "\0");
							strcpy(middle_str, "\0");
							strcpy(end_str, "\0");
						}
					}
				}
			}
		}
	}
}
void Step5(char *text)
{
	while (true)
	{
		bool time_to_end = true;
		char cur_nonterminal, left_nonterminal = text[0];
		int nonterminal_index, left_nonoterminal_index = 0;
		for (int i = 2; text[i] != '\0';)//ïîèñê îäèíî÷íîãî òåðìèíàëà ñïðàâà
		{
			int left_border, right_border;
			int nonterminal_count = 0;
			if (text[i] == '\n')
			{
				left_nonterminal = text[i + 1];
				left_nonoterminal_index = i + 1;
			}
			if (isLeftBorder(text[i]))
			{
				left_border = i;
				i++;
				while (!isRightBorder(text[i]))
				{
					right_border = i + 1;
					if ((text[i] >= 'A') && (text[i] <= 'Z'))
						nonterminal_count++;
					i++;
				}
				if (nonterminal_count == 1)
				{
					time_to_end = false;
					cur_nonterminal = text[right_border - 1];
					nonterminal_index = right_border - 1;
					break;
				}
			}
			else
				i++;
		}
		if (time_to_end)
			return;
		int begin_right_part, end_right_part = -1;
		for (int i = 0; text[i] != '\0'; i++)//ïîèñê ïðàâèëà
		{
			if ((text[i] == cur_nonterminal) && (text[i + 1] == '-'))
			{
				begin_right_part = i + 3;
				int j;
				for (j = i; text[j] != '\0'; j++)
				{
					if (text[j] == '\n')
					{
						end_right_part = j - 1;
						break;
					}
				}
				if (end_right_part == -1)
					end_right_part = j - 1;
				break;
			}
		}
		char middle_str[40];
		middle_str[0] = '\0';
		for (int i = begin_right_part; i <= end_right_part; i++)
		{
			int left_border, right_border;
			left_border = i - 1;
			while (!isRightBorder(text[i]))
			{
				right_border = i + 1;
				i++;
			}
			char new_rule[10];
			substr(new_rule, text, left_border + 1, right_border - 1);
			bool similar_left_and_right = false;
			if ((new_rule[0] == left_nonterminal) && (new_rule[1] == '\0'))
				similar_left_and_right = true;
			if ((isRuleNew(text, new_rule, left_nonoterminal_index + 3)) && (!similar_left_and_right))
			{
				strcat(middle_str, new_rule);
				strcat(middle_str, "|");
			}
		}
		if (middle_str[0] != '\0')
			middle_str[strlen(middle_str) - 1] = '\0';
		char begin_str[100];
		begin_substr(begin_str, text, nonterminal_index - 1);
		char end_str[100];
		int begin = nonterminal_index + 1;
		if ((middle_str[0] == '\0') && (text[nonterminal_index + 1] == '|'))
			begin = nonterminal_index + 2;
		end_substr(end_str, text, begin);
		strcat(begin_str, middle_str);
		strcat(begin_str, end_str);
		strcpy(text, begin_str);
	}
}
