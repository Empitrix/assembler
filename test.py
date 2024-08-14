# # data = "Awesome";
# # print(data[1:5])
# 
lines = [
	"string:  'Hello, World!' ; 10, 0"
]

# for i in range(len(lines)):

for i in range(len(lines)):                # PASS 1: do PER LINE replacements
	while(lines[i].find('\'') != -1):        # replace '...' occurances with corresponding ASCII code(s)
		k = lines[i].find('\'')
		l = lines[i].find('\'', k+1)
		if k != -1 and l != -1:
			replaced = ''
			for c in lines[i][k+1:l]: replaced += str(ord(c)) + ' '
			lines[i] = lines[i][0:k] + replaced + lines[i][l+1:]
		else: break

	if(lines[i].find(';') != -1): lines[i] = lines[i][0:lines[i].find(';')]	# delete comments

	print(f">{lines[i]}<");
