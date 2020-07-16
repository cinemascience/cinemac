#! /usr/bin/env python
import os
import pandas as pd 
import numpy as np
import plotly.graph_objects as go


class HaloTracing:
	""" A class that reads in a folder and traces halo lineage from .info files"""

	def __init__(self):
		self.halo_folder = ""
		self.cinema_folder = ""
		self.radius = 2
		self.halo_parent_child_id = []
		self.lineage = []
		self.halo_timesteps = []
		self.halo_tag_size_timestep = []


	def __init__(self, halo_info_folder, cinema_folder, radius):
		self.halo_folder = halo_info_folder
		self.cinema_folder = cinema_folder
		self.radius = radius
		self.halo_parent_child_id = []
		self.lineage = []
		self.halo_timesteps = []
		self.halo_tag_size_timestep = []


	def get_halo_information(self):
		return self.halo_info_df

	def get_lineage(self):
		return self.lineage


	def list_files_in_folder(self, folder, extension):
		"""List all the files in a folder"""
		file_list = []
		for file in os.listdir(folder):
			if file.endswith(extension):
				file_list.append(folder + "/" + file)

		file_list.sort()
		return file_list


	def read_info_file(self, filename):
		"""Reads a .info file"""
		row = []
		reader = open(filename)

		try:
			center = (reader.readline()).split(' ')
			row.append( center[1].split(',')[0] )
			row.append( center[2].split(',')[0] )
			row.append( center[3].split('\n')[0] )

			radius = (reader.readline()).split()
			row.append(radius[1])

			halo_id = (reader.readline()).split()
			row.append(halo_id[2])

			count = (reader.readline()).split()
			row.append(count[1])

			return row

		finally:
			reader.close()


	def create_db(self):
		""" Process files in folder for halos """
		# Get all the halo information files 
		file_list = self.list_files_in_folder(self.halo_folder,".info")

		# Process each file and store information
		rows = []
		ts_set = set() # set storing timestep infomration
		index = 0
		for filename in file_list:
			timestep = ((filename.split('-')[1]).split('_'))[0]
			ts_set.add(timestep)
   
			timestep_tag = (filename.split('-')[1]).split('.')[0]
			cinema_path = self.cinema_folder + "/m001-" + timestep_tag + ".cdb"

			row = self.read_info_file(filename)

			row.append(timestep)
			row.append(index)
			row.append(filename)
			row.append(cinema_path)

			index = index + 1

			rows.append(row)


		# Create a pandas dataframe from the list of list rows
		self.halo_info_df = pd.DataFrame(rows, columns =['x', 'y', 'z', 'radius', 'tag', 'size', 'timestep', 'id', 'filename', 'cinema_db'])
		self.timesteps = list(sorted(ts_set))
  
		#print(self.halo_info_df)



	def find_parent_child_relationship(self):
		"""Find parent child relationship between subsequent timesteps"""
		halo_descendant = []
		for i in range(len(self.timesteps)):
			halo_descendant_pair = []
			halo_descendant_pair_id = []
			if i == 0:
				continue

			temp_df_0 = self.halo_info_df.loc[self.halo_info_df['timestep'] == self.timesteps[i-1]]
			temp_df_1 = self.halo_info_df.loc[self.halo_info_df['timestep'] == self.timesteps[i]]


			temp_list_0 = temp_df_0.values.tolist()
			temp_list_1 = temp_df_1.values.tolist()

			index_0 = 0
			for row_0 in temp_list_0:
				pos_0_x = float(row_0[0])
				pos_0_y = float(row_0[1])
				pos_0_z = float(row_0[2])

				index_1 = 0
				for row_1 in temp_list_1:
					pos_1_x = float(row_1[0])
					pos_1_y = float(row_1[1])
					pos_1_z = float(row_1[2])

					dist_sq =  (pos_0_x - pos_1_x) * (pos_0_x - pos_1_x) + \
							(pos_0_y - pos_1_y) * (pos_0_y - pos_1_y) + \
							(pos_0_z - pos_1_z) * (pos_0_z - pos_1_z)

					if (dist_sq < self.radius*self.radius):
						halo_descendant_pair.append( [index_0,index_1] )
						halo_descendant_pair_id.append( [row_0[7],row_1[7]] )

					index_1 = index_1 + 1

				index_0 = index_0 + 1

			halo_descendant.append(halo_descendant_pair)
			self.halo_parent_child_id.append(halo_descendant_pair_id)

		#print("\nParent-Child:\n")
		#for pc in self.halo_parent_child_id:
		#	print (pc)



	def find_lineage(self):
		"""Find lineage for halos through the sim"""
		self.find_parent_child_relationship()
  


		first_time = True
		time_index = 0
		for row in self.halo_parent_child_id:
			# Add parent-child relationship for first two time steps
			if first_time:
				index = 0
				for cell in row:
					self.lineage.append(cell)
					self.halo_timesteps.append([time_index,time_index+1])
     
					self.halo_tag_size_timestep.append([])
					index_pos = (self.halo_info_df[self.halo_info_df['id']==cell[0]].index.values)[0]
					halo_tag_1 =  self.halo_info_df.at[index_pos,'tag']
					halo_size_1 = self.halo_info_df.at[index_pos,'size']
					halo_ts_1 = self.halo_info_df.at[index_pos,'timestep']
     
					index_pos = (self.halo_info_df[self.halo_info_df['id']==cell[1]].index.values)[0]
					halo_tag_2 =  self.halo_info_df.at[index_pos,'tag']
					halo_size_2 = self.halo_info_df.at[index_pos,'size']
					halo_ts_2 = self.halo_info_df.at[index_pos,'timestep']
					self.halo_tag_size_timestep[index].append([halo_tag_1,halo_size_1,halo_ts_1])
					self.halo_tag_size_timestep[index].append([halo_tag_2,halo_size_2,halo_ts_2])

					index = index + 1
     
				first_time = False
				time_index = time_index + 1
				continue

			# Trace for the rest
			for cell in row:
				appended = False
				index = 0
				for item in self.lineage:
					if cell[0] == item[ len(item)-1 ]:
						item.append(cell[1])
						self.halo_timesteps[index].append(time_index+1)

						index_pos = (self.halo_info_df[self.halo_info_df['id']==cell[1]].index.values)[0]
						halo_tag =  self.halo_info_df.at[index_pos,'tag']
						halo_size = self.halo_info_df.at[index_pos,'size']
						halo_ts = self.halo_info_df.at[index_pos,'timestep']
						self.halo_tag_size_timestep[index].append([halo_tag,halo_size,halo_ts])
     
						appended = True
					index = index + 1

				if (appended == False):
					self.lineage.append(cell)
					self.halo_timesteps.append([time_index,time_index+1])
     
					self.halo_tag_size_timestep.append([])
					index_pos = (self.halo_info_df[self.halo_info_df['id']==cell[0]].index.values)[0]
					halo_tag_1 =  self.halo_info_df.at[index_pos,'tag']
					halo_size_1 = self.halo_info_df.at[index_pos,'size']
					halo_ts_1 = self.halo_info_df.at[index_pos,'timestep']
     
					index_pos = (self.halo_info_df[self.halo_info_df['id']==cell[1]].index.values)[0]
					halo_tag_2 =  self.halo_info_df.at[index_pos,'tag']
					halo_size_2 = self.halo_info_df.at[index_pos,'size']
					halo_ts_2 = self.halo_info_df.at[index_pos,'timestep']
					self.halo_tag_size_timestep[index].append([halo_tag_1, halo_size_1, halo_ts_1])
					self.halo_tag_size_timestep[index].append([halo_tag_2, halo_size_2, halo_ts_2])
     
			time_index = time_index + 1
   

		#print("\nLineage:")
		#for l in self.lineage:
		#	print (l)
   
		#print("\nTimesteps: ")
		#for t in self.halo_timesteps:
		#	print(t)
   
		#print("\nhalo_tag_size_timestep: ")
		#for h in self.halo_tag_size_timestep:
		#	print(h)



	def get_position(self, row_to_trace):
		"""Get the position of halos taced"""
		pos_x = []
		pos_y = []
		pos_z = []
		#color_index = 
		for item in self.lineage[row_to_trace]:
			index = (self.halo_info_df[self.halo_info_df['id']==item].index.values)[0]
			
			pos_x.append(self.halo_info_df.at[index,'x'])
			pos_y.append(self.halo_info_df.at[index,'y'])
			pos_z.append(self.halo_info_df.at[index,'z'])

		return pos_x, pos_y, pos_z



	def plot_line(self, index):
		"""Draw one halo trace"""
		pos_x = []
		pos_y = []
		pos_z = []
		pos_x, pos_y, pos_z = self.get_position(index)

		df = pd.DataFrame(dict(
		    X=pos_x, 
		    Y=pos_y, 
		    Z=pos_z
		))

		fig = go.Figure()

		fig.add_trace(go.Scatter3d(
			x=pos_x, y=pos_y, z=pos_z,
				marker=dict(
					size=4,
					colorscale='Viridis',
				),
			line=dict(
				color='darkblue',
				width=2
			)
		))

		fig.show()



	def plot_lines(self):
		"""Draw all the halo traces"""
		fig = go.Figure()
        
		colormap = ['red', 'green', 'blue', 'orange', 'black', 
					'firebrick', 'limegreen', 'lightskyblue', 'yellow', 'gray', 
     				'pink', 'olive', 'brown', 'mediumblue', 'magenta', 'silver']
		
		for index in range(len(self.lineage)):
			pos_x = []
			pos_y = []
			pos_z = []
			pos_x, pos_y, pos_z = self.get_position(index)
   
			df = pd.DataFrame(dict(
			    X=pos_x, 
			    Y=pos_y, 
			    Z=pos_z
			))
   
			my_colors = []
			for i in range( len(self.halo_timesteps[index]) ):
				my_colors.append( colormap[ self.halo_timesteps[index][i] ] )

			my_hover_tag = []
			for i in range( len(self.halo_tag_size_timestep[index]) ):
				my_hover_tag.append("Tag: " + self.halo_tag_size_timestep[index][i][0] + "\n" 
                        			"Size: " + self.halo_tag_size_timestep[index][i][1] + "\n" 
                        			"ts: " + self.halo_tag_size_timestep[index][i][2])
    
			#print("tag:", my_hover_tag)
			
			fig.add_trace(go.Scatter3d(
					x=pos_x, y=pos_y, z=pos_z,
						marker=dict(
							size=4,
							color=my_colors
						),
					hovertext = my_hover_tag,
					line=dict(
						color='darkblue',
						width=2
					),
					name = "halo trace " + str(index)
				)
            )

		fig.show()
  
  
  
	def traceHalos(self):
		"""Creata DB from info and find relationship"""
		self.create_db()
		self.find_lineage()


#halo_tracing = HaloTracing("data/haloOutput_jun19_18","data/cinema_jun19_18", 2)
#halo_tracing.traceHalos()
#halo_tracing.plot_lines()