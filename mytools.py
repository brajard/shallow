def find_res(fname):
    fid = open(fname,'r')
    for l in fid:
        if 'Res=' in l:
            Ls= l.split()
            return(Ls[-1])
    return(0)
