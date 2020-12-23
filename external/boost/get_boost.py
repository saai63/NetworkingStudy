import requests
import os

def bIsDownloaded():
    if os.path.exists('_download_complete_'):
        return True
    else:
        return False


def touch(fname):
    if os.path.exists(fname):
        os.utime(fname, None)
    else:
        open(fname, 'a').close()


def get_proxies():
    http_proxy  = "http://127.0.0.1:3128"
    https_proxy = "http://127.0.0.1:3128"
    proxyDict = { 
              "http"  : http_proxy, 
              "https" : https_proxy
            }
    return proxyDict


def bDownloadBoost(boost_version):
    boost_version_str = boost_version.replace('.','_')
    url = 'https://sourceforge.net/projects/boost/files/boost/' + boost_version + '/boost_' + boost_version_str + '.tar.bz2'
    proxyDict = get_proxies();
    r = requests.get(url, proxies=proxyDict)
    if r.status_code != 200:
        print('Failed to download boost from URL : ' + url + ' with error code '+ str(r.status_code))
        return False
    else:
        fileName = 'boost_' +  boost_version_str + '.tar.bz2'
        open(fileName,'wb').write(r.content)
        touch('_download_complete_')
        return True


if __name__ == '__main__' :
    if bIsDownloaded():
        print ('Boost is already downloaded')
    else:
        print ('Boost is not downloaded. Download now')
        if bDownloadBoost('1.68.0'):
            print('Downloaded boost successfully')
        else:
            print('Downloaded of boost failed')

