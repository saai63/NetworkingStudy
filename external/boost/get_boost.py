import requests
from tqdm import tqdm
import os

BOOST_VERSION = '1.68.0'
DOWNLOAD_FLAG = '_download_complete_'

def bIsDownloaded():
    if os.path.exists(DOWNLOAD_FLAG):
        return True
    else:
        return False


def touch(fname):
    if os.path.exists(fname):
        os.utime(fname, None)
    else:
        open(fname, 'a').close()


def get_proxies():
    proxyDict = {}
    http_proxy = os.getenv('http_proxy')
    if http_proxy != None:
        proxyDict["http"] = http_proxy
    https_proxy = os.getenv('https_proxy')
    if https_proxy != None:
        proxyDict["https"] = https_proxy
    ftp_proxy = os.getenv('ftp_proxy')
    if ftp_proxy != None:
        proxyDict["ftp"] = ftp_proxy
    return proxyDict


def bDownloadBoost(boost_version):
    boost_version_str = boost_version.replace('.','_')
    url = 'https://sourceforge.net/projects/boost/files/boost/' + boost_version + '/boost_' + boost_version_str + '.tar.bz2'
    proxyDict = get_proxies();
    r = requests.get(url, proxies=proxyDict, stream=True)
    if r.status_code != 200:
        print('Failed to download boost from URL : ' + url + ' with error code '+ str(r.status_code))
        return False
    else:
        total_size_in_bytes= int(r.headers.get('content-length', 0))
        block_size = 1024 #1 Kilobyte
        fileName = 'boost_' +  boost_version_str + '.tar.bz2'
        progress_bar = tqdm(total=total_size_in_bytes, unit='iB', unit_scale=True)
        with open(fileName, 'wb') as file:
            for data in r.iter_content(block_size):
                progress_bar.update(len(data))
                file.write(data)
        progress_bar.close()
        #open(fileName,'wb').write(r.content)
        if total_size_in_bytes != 0 and progress_bar.n != total_size_in_bytes:
            print("ERROR, something went wrong")
            return False
        else:
            touch(DOWNLOAD_FLAG)
            return True


if __name__ == '__main__' :
    if bIsDownloaded():
        print ('Boost is already downloaded')
    else:
        print ('Boost is not downloaded. Download now')
        if bDownloadBoost(BOOST_VERSION):
            print('Downloaded boost successfully')
        else:
            print('Downloaded of boost failed')

