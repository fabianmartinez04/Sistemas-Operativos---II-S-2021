import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { CreateDriveComponent } from './components/create-drive/create-drive.component';
import { DashboardComponent } from './components/dashboard/dashboard.component';
import { UserComponent } from './components/user/user.component';

const routes: Routes = [
  {path: 'user-login', component: UserComponent},
  {path: 'drive-dashboard', component: DashboardComponent},
  {path: 'new-drive', component: CreateDriveComponent},
  {path: '**', redirectTo: 'user-login'}


];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
