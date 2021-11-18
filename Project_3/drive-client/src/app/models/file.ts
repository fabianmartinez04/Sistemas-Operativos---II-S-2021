export class File {
    fileName: string;
    FileExtension:string;
    type:string;
    dateCreated:string;
    dateModified:string;
    size:number;

    constructor() {
        this.fileName = '';
        this.FileExtension = '';
        this.type = '';
        this.dateCreated = '';
        this.dateModified = '';
        this.size = 0;
    }

    test(name:string,type:string){
        this.fileName = name;
        this.type= type;
    }
}