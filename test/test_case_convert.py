import os


def get_dirs(filename):
    dir_lists = os.listdir(filename)
    return dir_lists


def get_son_dirs(father_filename):
    all_dir2 = []
    path = father_filename
    for root, folders, files in os.walk(path):
        for file in files:
            all_dir2.append(os.path.join(root, file))
            pass
    return all_dir2


def last_fix_get(all_dirs):
    in_files = []
    out_files = []
    for i in all_dirs:
        if(i.endswith("in")):
            in_files.append(i)
        if i[:-5] == "input":
            in_files.append(i)
        if(i.endswith("out")):
            out_files.append(i)
    return in_files, out_files


def dirs_sync(in_dirs, out_dirs):
    synced_in_dirs = []
    synced_out_dirs = []
    for i in in_dirs:
        for j in out_dirs:
            if i.endswith("in"):
                if i[:-3] == j[:-4]:
                    synced_in_dirs.append(i)
                    synced_out_dirs.append(j)
                    break
            else:
                if i == j:
                    synced_in_dirs.append(i)
                    synced_out_dirs.append(j)
                    break
    return synced_in_dirs, synced_out_dirs


if __name__ == "__main__":
    dirs = get_dirs(".\\TestCase")
    all_dirs = get_son_dirs(".\\TestCase")
    in_file, out_file = last_fix_get(all_dirs)
    sync_in, sync_out = dirs_sync(in_file, out_file)
    print(sync_in)
