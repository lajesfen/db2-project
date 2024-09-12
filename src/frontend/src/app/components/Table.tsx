import React, { useEffect, useState } from 'react'
import {
    PaginationState,
    useReactTable,
    getCoreRowModel,
    getPaginationRowModel,
    ColumnDef,
    flexRender,
} from '@tanstack/react-table'

type Person = {
    firstName: string
    lastName: string
    age: number
    visits: number
    status: string
    progress: number
}

function getRandomPerson(id: number): Person {
    const firstNames = ['John', 'Jane', 'Mike', 'Emily', 'Robert', 'Alice', 'Chris', 'Tina', 'James', 'Jessica'];
    const lastNames = ['Smith', 'Johnson', 'Williams', 'Jones', 'Brown', 'Davis', 'Miller', 'Wilson', 'Taylor', 'Anderson'];
    const statuses = ['Single', 'In Relationship', 'Complicated', 'Married', 'Divorced'];

    return {
        firstName: firstNames[Math.floor(Math.random() * firstNames.length)],
        lastName: lastNames[Math.floor(Math.random() * lastNames.length)],
        age: Math.floor(Math.random() * 60) + 18, // Age between 18 and 77
        visits: Math.floor(Math.random() * 500),  // Random visit count
        status: statuses[Math.floor(Math.random() * statuses.length)],
        progress: Math.floor(Math.random() * 100), // Progress between 0 and 100
    };
}

function generateLargeDataset(size: number): Person[] {
    const data: Person[] = [];
    for (let i = 0; i < size; i++) {
        data.push(getRandomPerson(i));
    }
    return data;
}

export default function Table() {
    const [data, setData] = useState<Person[]>([]);

    useEffect(() => {
        setData(generateLargeDataset(1000));
    }, [])

    const [pagination, setPagination] = React.useState<PaginationState>({
        pageIndex: 0,
        pageSize: 10,
    })

    const columns = React.useMemo<ColumnDef<Person>[]>(
        () => [
            {
                accessorKey: 'n',
                header: () => '#',
                cell: info => info.row.index,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'firstName',
                header: () => `First Name`,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'lastName',
                header: () => `Last Name`,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'age',
                header: () => `Age`,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'visits',
                header: () => `Visits`,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'status',
                header: () => `Status`,
                footer: props => props.column.id,
            },
            {
                accessorKey: 'progress',
                header: () => `Progress`,
                footer: props => props.column.id,
            },

        ],
        []
    )

    const table = useReactTable({
        data,
        columns,
        state: {
            pagination,
        },
        onPaginationChange: setPagination,
        // Pipeline
        getCoreRowModel: getCoreRowModel(),
        getPaginationRowModel: getPaginationRowModel(),
        debugTable: true,
    })

    return (
        <div className="p-2 w-full overflow-x-scroll overflow-y-hidden">
            {data.length === 0 ? (
                <div className='flex justify-center'>
                    <p>Loading Data...</p>
                </div>
            ) : (
                <table className="w-full text-center table-fixed">
                    <thead>
                        {table.getHeaderGroups().map(headerGroup => (
                            <tr key={headerGroup.id}>
                                {headerGroup.headers.map(header => {
                                    return (
                                        <th key={header.id} className={`border ${header.id == 'n' ? 'w-10' : ''}`}>
                                            {header.isPlaceholder ? null : (
                                                <div>
                                                    {flexRender(
                                                        header.column.columnDef.header,
                                                        header.getContext()
                                                    )}
                                                </div>
                                            )}
                                        </th>
                                    )
                                })}
                            </tr>
                        ))}
                    </thead>
                    <tbody className='text-sm'>
                        {table.getRowModel().rows.map(row => {
                            return (
                                <tr key={row.id}>
                                    {row.getVisibleCells().map(cell => {
                                        return (
                                            <td key={cell.id} className='border py-1'>
                                                {flexRender(
                                                    cell.column.columnDef.cell,
                                                    cell.getContext()
                                                )}
                                            </td>
                                        )
                                    })}
                                </tr>
                            )
                        })}
                    </tbody>
                </table>
            )}
            <div className='flex flex-col mt-8'>
                <div className="flex items-center gap-2">
                    <button
                        className="border rounded p-1"
                        onClick={() => table.setPageIndex(0)}
                        disabled={!table.getCanPreviousPage()}
                    >
                        {'<<'}
                    </button>
                    <button
                        className="border rounded p-1"
                        onClick={() => table.previousPage()}
                        disabled={!table.getCanPreviousPage()}
                    >
                        {'<'}
                    </button>
                    <button
                        className="border rounded p-1"
                        onClick={() => table.nextPage()}
                        disabled={!table.getCanNextPage()}
                    >
                        {'>'}
                    </button>
                    <button
                        className="border rounded p-1"
                        onClick={() => table.setPageIndex(table.getPageCount() - 1)}
                        disabled={!table.getCanNextPage()}
                    >
                        {'>>'}
                    </button>
                    <span className="flex items-center gap-1">
                        <div>Page</div>
                        <strong>
                            {table.getState().pagination.pageIndex + 1} of{' '}
                            {table.getPageCount()}
                        </strong>
                    </span>
                    <span className="flex items-center gap-1">
                        | Go to page:
                        <input
                            type="number"
                            min="1"
                            max={table.getPageCount()}
                            defaultValue={table.getState().pagination.pageIndex + 1}
                            onChange={e => {
                                const page = e.target.value ? Number(e.target.value) - 1 : 0
                                table.setPageIndex(page)
                            }}
                            className="border p-1 rounded w-16"
                        />
                    </span>
                    <select
                        value={table.getState().pagination.pageSize}
                        onChange={e => {
                            table.setPageSize(Number(e.target.value))
                        }}
                    >
                        {[10, 20, 30, 40, 50].map(pageSize => (
                            <option key={pageSize} value={pageSize}>
                                Show {pageSize}
                            </option>
                        ))}
                    </select>
                </div>
                <div>{table.getRowModel().rows.length} Rows</div>
            </div>
        </div>
    )
}