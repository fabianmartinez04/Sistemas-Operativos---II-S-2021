export class File {
    fileName: string;
    Fileextension:string;
    type:string;
    dateCreated:string;
    dateModified:string;
    size:number;

    constructor() {
        this.fileName = '';
        this.Fileextension = '';
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