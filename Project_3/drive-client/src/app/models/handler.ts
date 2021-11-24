import { File } from "./file";

export class Handler {
    // load initial files
    
    loadFileOfPath(folder: any,personalFiles:Boolean) : File[] {
        let children : JSON[] = folder.children;

        let files : File[] = [];
        children.forEach((element:any) => {
            let file : File = new  File();
            if(!personalFiles){
                file.owner = element.owner;
            }
            file.type = element.type;
            file.fileName = element.name;
            file.route = element.route;
            file.dateCreated = element.dateCreated;
            file.size = element.size;

            if(file.type == 'file') {
                file.FileExtension = element.extension;
                file.dateModified = element.dateModified;
                file.text = element.text;
            }

            files.push(file)
        });

        return files;

    }
}
