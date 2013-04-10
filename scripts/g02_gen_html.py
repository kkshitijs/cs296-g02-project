#This file parses a tex file, and generates an html file based on a part of the parsed text

#Module needed for regex functionalities
import re

#Custom defined list of tags in Latex

latex_tags = [ "\\" , "\\_" , "\\emph" , "\\textbf" , "\\textbf{\\emph"]
html_tags = [ " ", " " , "<em>" , "<b>" , "<b><em>"]
margin_top = ""
margin_left = ""
margin_right = ""
margin_bottom = ""
author = []
roll_no = []
email_id = []
a = -1

#Function to handle inline $s
def dollar_handler(line):

	print("LINE IS--------", line)
	match = re.search("(.*)(?<=\$)([^\$]*)(?=\$)(.*)", line)
	if match:
		word = match.group(2)
		print("WORD IS-",word, "----")
		if word == "":
			return line
		match2 = re.search(".*approx(.*)", word)
		if match2:
#			match = re.search("(.*)$(.*)$(.*)", line)
			line = match.group(1)[:-1]
			line += " ~"
			line += match2.group(1)
			line += match.group(3)[1:]
		else:
#			match = re.search("(.*)$(.*)$(.*)", line)
#			line = ""
			line = match.group(1)[:-1]
#			print("Adding to line" , 
#			line += "~"
			line += match.group(3)[1:]
#			line += match.group(3)
	
#	print("NEW LINE:", line)
	return line				

#Function to handle images in the tex file
def image_handler(html_file, line):

	img = ""
	width = 0
	height = 0
	match = re.search("(.*)width=(.*)mm(.*)height=(.*)mm(.*){(.*).eps}" , line)
	if match:
		img = match.group(6)
		width = int(match.group(2))
		height = int(match.group(4))	
	html_file.write("""<figure><img src="../doc/{0}.png" width="{1}" height="{2}"></figure>""".format(img, 5*width, 5*height))
	
#Function to handle lists in the tex file
def list_handler(tex_file, html_file, line):

	html_file.write("<ul>\n")
	while True:
		line = tex_file.readline()
		line = dollar_handler(line)
		line = ignore(html_file, line)
		
		if line[0:16] == "\\includegraphics":
			image_handler(html_file, line)
			line = tex_file.readline()
			line = ignore(html_file, line)
			line = dollar_handler(line)
		if line[0:4] == "\\end":
			break
		if line[0:6] == "\\begin":
			match = re.search("begin{(.*)}" , line)
			if match:
				if match.group(1) == "itemize":
					list_handler(tex_file, html_file, line)
				elif match.group(1) == "center":
					quote_center_handler(tex_file, html_file, line, "center")	
				elif match.group(1) == "quote":
					quote_center_handler(tex_file, html_file, line, "quote")	
		match = re.search("\\item (.*)" , line)	
		if match:
			html_syntax = tag_replace(match.group(1))
			html_file.write("<li>")
			html_file.write(html_syntax)
			html_file.write("</li>\n")

	html_file.write("</ul>\n")	

#Function to handle quotes from tex file
def quote_center_handler(tex_file, html_file, line, tag):

	if tag=="center":
		html_file.write("<center>\n")
	elif tag=="quote":
		html_file.write("<br><br><i>\n")	
	while True:
		line = tex_file.readline()
		line = ignore(html_file, line)
		line = dollar_handler(line)
		if line[0:16] == "\\includegraphics":
			image_handler(html_file, line)
			line = tex_file.readline()
			line = ignore(html_file, line)
			line = dollar_handler(line)
		if line[0:4] == "\\end":
			break
		if line[0:6] == "\\begin":
			match = re.search("begin{(.*)}" , line)
			if match:
				if match.group(1) == "itemize":
					list_handler(tex_file, html_file, line)
				elif match.group(1) == "center":
					quote_center_handler(tex_file, html_file, line, "center")	
				elif match.group(1) == "quote":
					quote_center_handler(tex_file, html_file, line, "quote")	
		html_syntax = tag_replace(line)
		html_file.write(html_syntax)
			
	if tag=="center":
		html_file.write("</center>\n")
	elif tag=="quote":
		html_file.write("</i>\n")

#Function to ignore the comments and empty new lines
def ignore(html_file, line):

	line1 = line
	while True:
		if line[0:1] == "%" or line == "\n":
#			print("Ignored ", line)
			line = tex_file.readline()
			line = dollar_handler(line)
			line1 = line
		else:
			break

	return line1
	
#This function takes a word and recognises whether this is a special tag in Latex
def tag_replace(string):

#	while string[0] == " ":
#		string = string[1:]
	
	word_list = string.split(' ')
#	print(word_list[-1])
#	if word_list[-1] != "" and word_list[-1][-1] == '\n':
#		word_list[-1] == word_list[-1][:-2]
#		print("CHANGED----------", word_list[-1])
#	print(word_list)
	html_syntax = " "
	for word in word_list:
		if word == '':
			continue
		if word[-1] == '\n':
			word = word[:-1]
		if word == '':
			continue

		if word[0] == '\\':
#			print("word[0] is---------" , word[0] , "for" , word)


			replaced = ""
			temp = ""
			match = re.search("(.*){" , word)
			if match:
#				print("---------------" , match.group())
#				print("---------------" , match.group(1))
				index = latex_tags.index(match.group(1))
				replaced += html_tags[index]
				match = re.search(".*{(.*)" , word)
				if match:
#					print("#########---------------" , match.group())
#					print("#########---------------" , match.group(1))
					replaced += match.group(1)
					if match.group()[-1] == '}':
						a=""
#						temp = html_tags[index]
						replaced += invert(html_tags[index])
					else:
						temp = html_tags[index]
#					replaced += invert(html_tags[index])

			html_syntax += replace(word)
			html_syntax += " "

		elif word[-1] == '}':
#			print("temp is now" , temp)
			if temp != "":
#				print("^^^^^^^^^^^^^^---------------" , word)
#				print("^^^^^^^^^^^^^^---------------" , word[:-1])
				while word[-1] == '}':
					word = word[:-1]
				if word[-1] == '$':
					word = word[:-1]
					while word[-1] != '$':
						word = word[:-1]
					word = word[:-1]					
				html_syntax += word
				html_syntax += " "		
				html_syntax += invert(temp)
			
		else:
			html_syntax += word
			html_syntax += " "		


	return html_syntax			

#This function takes a tag from Latex and converts it into its corresponding html tag
def replace(word):

	replaced = ""
	match = re.search("(.*){" , word)
	if match:
#		print("---------------" , match.group())
#		print("---------------" , match.group(1))
		index = latex_tags.index(match.group(1))
		replaced += html_tags[index]
		
		match = re.search(".*{(.*)}" , word)
		if match:
			replaced += match.group(1)
			replaced += invert(html_tags[index])
		else:
			match = re.search(".*{(.*)" , word)
			replaced += match.group(1)
#			replaced += invert(html_tags[index])
	return replaced

#Searches and replaces tags in tags in the latex code, to corresponding html code
def tag_in_tag_search(word):

	new_word = ""
	match = re.search("(.*)\\(.*)[{ ]" ,word)
	if match:
		new_word += match.group(1)
		index = latex_tags.index(match.group(2))
		new_word += html_tags[index]
		match = re.search(".*{(.*)}" , word)
		if match:
			replaced += match.group(1)
			replaced += invert(html_tags[index])
		if match.group(2) != '':
			replaced += tag_in_tag_search(match.group(2))
	else:
		new_word += word	
	
	return new_word
	
#This function takes an html tag and inverts it , i.e., closes it in the required syntax
def invert(html_tag):

	inverted = ""
	for i in range(len(html_tag)):
		if(html_tag[i] == '<'):
			inverted += "</"
		elif(html_tag[i] == '>'):
			inverted += ">"	
		else:
			inverted += html_tag[i]	
	return inverted
					
#This function pre-processes the html file to generate the required syntax
def pre_process(f):

	f.write("<!DOCTYPE html>\n")
	f.write("<html>\n")
	f.write("<head>\n")
	f.write("<title> Profiling Report </title>\n")
	f.write('''<style type="text/css">\n''')
	f.write('''body {color:black; font-family:arial; font-size:18px; background-color:lavender}\n''')
	f.write("</style>")
	f.write("</head>\n")
	f.write("""<body leftmargin="{0}" topmargin="{1}" >\n""".format(margin_left, margin_top))

#This function post-processes the html file to finish it with the required syntax
def post_process(f):

	f.write("<center>-*-*-*-*-</center>\n")
	f.write("</body>\n")
	f.write("</html>")

#----------------------------------Main Program Starts Here---------------------------------

print("The Profiling Report tex file being considered is : g02_prof_report.tex")

tex_file = open("g02_project_report.tex" , 'r')
html_file = open("g02_timing_report.html" , 'w')

print("Opened File!")

line = ""
start_writing = 0
new_sect = 0
timing_section = 0
while True:

	if new_sect == 1:
		new_sect = 0
	elif new_sect == 0:
		line = tex_file.readline()
		line = dollar_handler(line)

	line = dollar_handler(line)
	line = ignore(html_file, line)
	

	if line[0:16] == "\\includegraphics":
		image_handler(html_file, line)
		line = tex_file.readline()
		line = dollar_handler(line)
		print("TAKING IN-----" , line)
	

	if len(line) == 0:
#		print("GOING OUT OF LOOP")
		break
	
#Ignoring comments here	
	elif line[0:1] == "%":
#		print("Ignored ", line)
		continue
	
	elif line[0:8] == "\\section":
		start_writing = 1
		match = re.search("section{(.*)}", line)
		if match:
#			print("Writing", match.group(1))
			if ((match.group(1)).split(' '))[0] == "Timing":
				timing_section = 1
#				while not(line[0:8] == "\\section"):
#					line = tex_file.readline()
#					new_sect = 1
#				continue	
			else:
				timing_section = 0
			if ((match.group(1)).split(' '))[0] == "A" and ((match.group(1)).split(' '))[1] == "Thousand":
				break
			html_syntax = tag_replace(match.group(1))
			html_file.write("<h1>")
			html_file.write("<u><center>")
			html_file.write(html_syntax)
		html_file.write("</center></u></h1>\n")
		continue
		
	elif line[0:11] == "\\subsection":
		start_writing = 1
		html_file.write("<h2>")
		match = re.search("section{(.*)}", line)
		if match:
#			print("Writing", match.group(1))
			html_syntax = tag_replace(match.group(1))
			html_file.write(html_syntax)
		html_file.write("</h2><hr>\n")
#		html_file.write("<hr>\n")
		continue
	
	elif line[0:6] == "\\begin":
		start_writing = 1
		match = re.search("begin{(.*)}" , line)
		if match:
#			print("Found for BEGIN" , match.group(1))
			if match.group(1) == "itemize" and timing_section == 1:
				
				html_file.write("<ul>\n")
				while True:
#					plot_no += 1
					a += 1
					line = tex_file.readline()
					line = ignore(html_file, line)
					line = dollar_handler(line)
					if line[0:4] == "\\end":
						break
					match = re.search("\\item (.*)" , line)	
					if match:
						html_syntax = tag_replace(match.group(1))
						html_file.write("<li>")
						html_file.write(html_syntax)
						html_file.write("</li>\n")
						if a==0:
							html_file.write('''<center><img src="../plots/g02_project_plot01.png" width=48% height=40%></center>\n''')
						elif a==2:
							html_file.write('''<center><img src="../plots/g02_project_plot00.png" width=400px height=400px></center>\n''')
						elif a==3:
							html_file.write('''<left><img src="../plots/g02_project_plot02.png" width=48% height=40% background-color:transparent></left>\n''')
							html_file.write('''<right><img src="../plots/g02_project_plot03.png" width=48% height=40% background-color:transparent></right>\n<br>\n''')
						elif a==4:
							html_file.write('''<center><img src="../plots/g02_project_plot05.png" width=400px height=400px></center>\n''')
						
				html_file.write("</ul>\n")						

			elif match.group(1) == "itemize":
				list_handler(tex_file, html_file, line)	
			elif match.group(1) == "quote":
				quote_center_handler(tex_file, html_file, line, "quote")
			elif match.group(1) == "center":
				quote_center_handler(tex_file, html_file, line, "center")
			elif match.group(1) == "document":
				start_writing = 0
				
				#Handling the title of the document
				html_file.write("<h1><center><b><u>")
				line = tex_file.readline()
#				print("@#%@#%@#FOUND--------" , line)
				
				line = ignore(html_file, line)
				line = dollar_handler(line)
#				print("@#%@#%@#FOUND--------" , line)
				match = re.search("{(.*)}" , line)
#				print("@#%@#%@#FOUND--------" , match.group())
				
				html_syntax = tag_replace(match.group(1))
				html_file.write(html_syntax)
				html_file.write("</u></b></center></h1>\n")
				
				#Handling the authors, roll_no's and email_ids
				line = tex_file.readline()
				line = ignore(html_file, line)
				line = dollar_handler(line)
				if line[0:7] == "\\author":
				
#					print("REACHED AUTHOR", line)
					while True:
						if line[0:1] == '}':
							break
						else:
							line = tex_file.readline()
							line = ignore(html_file, line)
							line = dollar_handler(line)
#							print("LINE IS", line)
							match = re.search("[A-Za-z ]+", line)
#							print("MATCH IS", match.group())
							author.append(match.group())
							line = tex_file.readline()	
							line = ignore(html_file, line)
							line = dollar_handler(line)
							match = re.search("[0-9A-Za-z ]+" , line)
							roll_no.append(match.group())
							line = tex_file.readline()
							line = ignore(html_file, line)
							line = dollar_handler(line)	
							match = re.search("[A-Za-z.-:]+@[A-Za-z.-:]+" , line)
							email_id.append(match.group())
							line = tex_file.readline()
							line = ignore(html_file, line)
							line = dollar_handler(line)
				
#				print(author)
#				print(roll_no)
#				print(email_id)			
				for i in range(len(author)):
				
					html_file.write("""<div style="position:absolute; left:{0}%; align:center;">""".format(100*(i+1)/(len(author)+1)))
					html_file.write(author[i])
					html_file.write("<br></div>\n")					
				html_file.write("<br>\n")	
				for i in range(len(roll_no)):
				
					html_file.write("""<div style="position:absolute; left:{0}%;">""".format(100*(i+1)/(len(roll_no)+1)))
					html_file.write(roll_no[i])
					html_file.write("<br></div>\n")
				html_file.write("<br>\n")					
				for i in range(len(email_id)):
				
					html_file.write("""<div style="position:absolute; left:{0}%;">""".format(100*(i+1)/(len(email_id)+1)))
					html_file.write(email_id[i])
					html_file.write("<br></div>\n")
				html_file.write("<br>\n")		
				html_file.write("<br>\n")						
				line = tex_file.readline()
				line = ignore(html_file, line)
				line = dollar_handler(line)
	
				if line[0:5] == "\\date":
				
					html_file.write("<center><SCRIPT>document.write(Date())</SCRIPT></center>\n")
					
					
			
		continue

#Formatting taken from here, extracted from the Tex File	
	elif line[0:12] == "\\usepackage[":
		match = re.search("top=(.*), b", line)
		if match:
			margin_top = match.group(1)
		match = re.search("bottom=(.*), l", line)
		if match:
			margin_bottom = match.group(1)
		match = re.search("left=(.*), r", line)
		if match:
			margin_left = match.group(1)
		match = re.search("right=(.*)]", line)
		if match:
			margin_right = match.group(1)
		pre_process(html_file)
		continue	
			
	
	else:
		if start_writing == 1:
			line = dollar_handler(line)
			html_syntax = tag_replace(line)
			html_file.write(html_syntax)			
		
post_process(html_file)

print("Closing File!!")
tex_file.close()
html_file.close()				 
