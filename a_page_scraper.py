import urllib.request

valid = []

def getHtml(atUrl):
    with urllib.request.urlopen(atUrl) as response:
        html = response.read()
    return html.decode("utf-8")

def genStr(baseAddr, currStr, maxLength, searchStr):
    print(currStr)
    if len(currStr) <= maxLength:
        try:
            html = getHtml(baseAddr + currStr + '.html')
            valid.append(currStr)
            print (baseAddr, currStr, '.html')
        except:
            print('', sep='', end='')

        for i in range(48, 58):
            genStr(baseAddr, currStr + chr(i), maxLength, searchStr)

def search(validUrls, searchStr):
    for x in validUrls:
        html = getHtml(x)
        
        if searchStr in html:
            print(x)

f = open('validurls.txt', 'r')

lines = f.readlines()

search(lines, 'grinding wheel')