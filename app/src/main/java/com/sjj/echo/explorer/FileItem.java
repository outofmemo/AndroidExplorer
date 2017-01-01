package com.sjj.echo.explorer;

/**
 * Created by SJJ on 2016/12/7.
 */
public class FileItem {
    String mName = null;
    String mTime = "";
    long mSize = 0;
    String mPrem = "";
    String mLink = "";
    boolean mIsDir = false;
    boolean mSelected = false;
    public FileItem(String name,String time,long size,String prem,String link,boolean isDir)
    {
        this.mName = name;
        this.mIsDir = isDir;
        if(time!=null)
            this.mTime = time;
        this.mSize = size;
        if(prem!=null)
            this.mPrem = prem;
        if(link!=null)
            this.mLink = link;
    }
}
