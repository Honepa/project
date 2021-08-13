from num2words import num2words

saved_num = []

def count_space(word) :
    counter = 0
    for i in range(len(word)):
        if word[i] == " ":
            counter += 1
    return counter

def save_num(num) :
    if len(saved_num) <= 8 :
        saved_num.append(num)
    else:
        for i in range(len(saved_num) - 1):
            saved_num[i] = saved_num[i + 1]
        saved_num[len(saved_num) - 1] = num

def chek_recur():
    if len(saved_num) != 9:
        return 0
    else:
        # ru
        num1 = saved_num[0] + saved_num[1] + saved_num[2]
        num2 = saved_num[3] + saved_num[4] + saved_num[5]
        num3 = saved_num[6] + saved_num[7] + saved_num[8]
        #fr
        #num1 = saved_num[0] + saved_num[1] + saved_num[2] + saved_num[3]
        #num2 = saved_num[4] + saved_num[5] + saved_num[6] + saved_num[7]
        if num1 == num2 == num3:
            return 1
        else:
            return 0

def words_and_words(num): 
   word = num2words(num, lang='ru')
   new_num = len(word) - word.count(" ")
   save_num(new_num)
   print(word + " " + str(new_num))
   if chek_recur():
       return 1
   else:
       words_and_words(new_num)
   
if __name__ == '__main__':
    words_and_words(10000)
    print(saved_num)
