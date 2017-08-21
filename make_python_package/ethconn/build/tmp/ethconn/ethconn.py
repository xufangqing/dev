# -*- coding: utf-8 -*-
'''
Created on 2015年12月11日

@author: 10091192
'''
import urllib
import urllib2
import getpass
# # 创建一个密码管理者 
# password_mgr = urllib2.HTTPPasswordMgrWithDefaultRealm() 
# # 添加用户名和密码 
# # 如果知道 realm, 我们可以使用他代替 ``None``. 
# top_level_url = "http://example.com/foo/" 
# password_mgr.add_password(None, top_level_url, username, password) 
# handler = urllib2.HTTPBasicAuthHandler(password_mgr) 
# # 创建 "opener" (OpenerDirector 实例) 
# opener = urllib2.build_opener(handler) 
# # 使用 opener 获取一个URL 
# opener.open(a_url) 
# # 安装 opener. 
# # 现在所有调用 urllib2.urlopen 将用我们的 opener. 
# urllib2.install_opener(opener)



# enable_proxy = True
# proxy_handler = urllib2.ProxyHandler({"http" : 'http://some-proxy.com:8080'})
# null_proxy_handler = urllib2.ProxyHandler({})
# if enable_proxy:
#     opener = urllib2.build_opener(proxy_handler)
# else:
#     opener = urllib2.build_opener(null_proxy_handler)
# urllib2.install_opener(opener)


# class RedirectHandler(urllib2.HTTPRedirectHandler):
#     def http_error_301(self, req, fp, code, msg, headers):
#         pass
#     def http_error_302(self, req, fp, code, msg, headers):
#         pass
#  
# opener = urllib2.build_opener(RedirectHandler)
# opener.open('http://www.google.cn')

# import cookielib
# cookie = cookielib.CookieJar()
# opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookie))
# response = opener.open('http://www.google.com')
# for item in cookie:
#     if item.name == 'some_cookie_item_name':
#         print item.value

def main():
    username=raw_input("please input you username:")
    password=getpass.getpass("password:")
    opener = urllib2.build_opener(urllib2.ProxyHandler({}))
    urllib2.install_opener(opener)
    url = r'http://10.3.76.100/'
    url = r'http://10.3.76.100/ac_portal/login.php'
    a_headers = {
                 "Accept":r'*/*',
                'Accept-Encoding':r'gzip,deflate,sdch',
                'Accept-Language':r'zh-CN,zh;q=0.8',
                'Connection':r'keep-alive',
#                'Content-Length':r'69',
                'Content-Type':r'application/x-www-form-urlencoded; charset=UTF-8',
                'Host':r'10.3.76.100',
                'Origin':r'http://10.3.76.100',
                'Referer':r'http://10.3.76.100/ac_portal/zte_webauth/pc.html?tabs=pwd',
                'User-Agent':r'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.57 Safari/537.36',
                'X-Requested-With':'XMLHttpRequest',
                 }
    values = {
              'opr':'pwdLogin',
              'lang':'chs',
              'userName':username,
              'pwd':password,
#              'rememberPwd':'0',
              } 
    a_data = urllib.urlencode(values) 
    req = urllib2.Request(url, data=a_data, headers=a_headers) 
#     req = urllib2.Request(url)
    
    try:
        response=urllib2.urlopen(req)
    except urllib2.HTTPError,e:    #HTTPError必须排在URLError的前面
        print "HTTPError:The server couldn't fulfill the request"
        print "Error code:",e.code
        print "Return content:",e.reason 
        print "html info:\n",e.read()
        print "info:",e.info()
        print "get_url:",e.geturl()
    except urllib2.URLError,e:
        if hasattr(e,"reason"):
            print "Failed to reach the server"
            print "The reason:",e.reason
        elif hasattr(e,"code"):
            print "The server couldn't fulfill the request"
            print "Error code:",e.code
            print "Return content:",e.read()
    else:
        pass  #其他异常的处理
    print response
    print response.read()
    print "overover"
    
# Request URL:http://10.3.76.100/
# Request Method:POST
# Status Code:200 OK
# 
# 
# Request Headersview source
# 
# Accept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
# Accept-Encoding:gzip,deflate,sdch
# Accept-Language:zh-CN,zh;q=0.8
# Cache-Control:max-age=0
# Connection:keep-alive
# Content-Length:190
# Content-Type:application/x-www-form-urlencoded
# Cookie:zteauthuser=10091192
# Host:10.3.76.100
# Origin:http://10.3.76.100
# Referer:http://10.3.76.100/
# User-Agent:Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.57 Safari/537.36
# 
# 
# Form Dataview sourceview URL encoded
# 
# %%MAGICID%%:%%MAGICVAL%%
# lang:chs
# username:10091192
# encryptToken:
# passwd:********
# pwd:00149c2d007439200002796f0071cfa00025df5e004f6ad4004a0e45000cade80018ae79004a0e450018ae79
# 
# 
# Response Headersview source
# Cache-Control:no-cache,must-revalidate
# Connection:close
# Content-Length:445
# Content-Type:text/html; charset=utf-8
# Date:Wed, 16 Dec 2015 01:33:09 GMT
# Last-Modified:Wed, 16 Dec 2015 01:33:09 GMT
# Server:SinforHttpd/1.0

if __name__ == '__main__':
    main()
    


