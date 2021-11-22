export class File {
    fileName: string;
    FileExtension:string;
    type:string;
    dateCreated:string;
    dateModified:string;
    size:number;
    text:string;
    route:string;

    constructor() {
        this.fileName = '';
        this.FileExtension = '';
        this.type = '';
        this.dateCreated = '';
        this.dateModified = '';
        this.size = 0;
        this.text = '';
        this.route = '';
    }
}